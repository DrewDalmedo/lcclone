#!/bin/sh
# tests/e2e.sh: end-to-end tests of the lc command line.
#
#   sh tests/e2e.sh ./lc             run from the repository root
#   UPDATE=1 sh tests/e2e.sh ./lc    rewrite the golden files instead
#
# Most tests compare a run's stdout, stderr and exit status with a golden
# file in tests/expected/. In those files the repository's absolute path is
# replaced by @ROOT@, the scratch directory by @TMP@ and the escape
# character by <ESC>, so they are portable. Every run gets a controlled
# environment (env -i): COLUMNS=80, TERM=xterm, LC_ALL=C, HOME pointing at an
# empty directory, LC_PROBLEMS set explicitly and no NO_COLOR or PAGER.
#
# LC_E2E_WRAP runs every lc under a wrapper, for example
#   LC_E2E_WRAP='valgrind -q --leak-check=full --error-exitcode=9'
#
# The out-of-memory tests preload an allocator built with $CC (default cc)
# and need LD_PRELOAD; the terminal tests need python3. Where they can't
# run they are skipped with a note.
#
# POSIX sh only: this runs under dash.

LC=${1:-./lc}
case $LC in
/*) ;;
*) LC=$(pwd)/$LC ;;
esac
if [ ! -f tests/e2e.sh ]; then
    echo "e2e.sh: run this from the repository root" >&2
    exit 2
fi
if [ ! -x "$LC" ]; then
    echo "e2e.sh: '$LC' is not executable" >&2
    exit 2
fi

LC_ALL=C
export LC_ALL
ROOT=$(pwd -P)
EXP=tests/expected
FIX=tests/fixtures/problems
CLIFIX=tests/fixtures/cli
TMP=$(mktemp -d "${TMPDIR:-/tmp}/lc-e2e.XXXXXX" 2>/dev/null) || {
    TMP=${TMPDIR:-/tmp}/lc-e2e.$$
    mkdir "$TMP" || exit 2
}
TMP_P=$(cd "$TMP" && pwd -P)
trap 'rm -rf "$TMP"' EXIT
trap 'exit 130' INT TERM
mkdir "$TMP/home" "$TMP/empty"
mkdir -p "$EXP"
ESC=$(printf '\033')
PYTHON=$(command -v python3 2>/dev/null)

pass=0
fail=0
updated=0

ok() {
    pass=$((pass + 1))
}

bad() {
    fail=$((fail + 1))
    echo "FAIL: $1: $2"
}

# A string as a basic regular expression for sed's s/// command.
re_escape() {
    printf '%s\n' "$1" | sed 's/[]\/$*.^[]/\\&/g'
}
ROOT_RE=$(re_escape "$ROOT")
TMP_RE=$(re_escape "$TMP")
TMP_P_RE=$(re_escape "$TMP_P")

# The root is replaced only where a path into it, or the root itself, is
# (so a root of /w doesn't turn @TMP@/ws into @TMP@@ROOT@s). Times and
# sizes of runs differ from one run to the next.
normalize() {
    sed -e "s/$TMP_P_RE/@TMP@/g" -e "s/$TMP_RE/@TMP@/g" \
        -e "s/$ROOT_RE\\([/\"']\\)/@ROOT@\\1/g" -e "s/$ROOT_RE\$/@ROOT@/" \
        -e "s/$ESC/<ESC>/g" \
        -e 's/[0-9][0-9]* ms/N ms/g' -e 's/[0-9][0-9.]* MB/N MB/g'
}

# The compilers lc uses for solutions: the ones make was given, if any.
E2E_CC=${CC:-cc}
E2E_CXX=${CXX:-c++}

# The LC_E2E_WRAP program as an absolute path, because some tests give lc a
# PATH of their own, and env may look the program up in that.
WRAP_PROG=
WRAP_ARGS=
if [ -n "${LC_E2E_WRAP-}" ]; then
    # shellcheck disable=SC2086 # the wrapper is split into words
    set -- $LC_E2E_WRAP
    WRAP_PROG=$(command -v "$1") || {
        echo "e2e.sh: LC_E2E_WRAP: '$1' not found" >&2
        exit 2
    }
    shift
    WRAP_ARGS=$*
fi

# lc_env [VAR=value...] -- ARGS...: runs lc with the test environment plus
# the given variables. A variable set to "" counts as unset for lc. With
# PTY_COLS set, lc runs on a terminal (see pty_run).
lc_env() {
    rotated=
    for a do
        shift
        if [ -z "$rotated" ] && [ "x$a" = x-- ]; then
            if [ -n "$WRAP_PROG" ]; then
                # shellcheck disable=SC2086 # split into words, as given
                set -- "$@" "$WRAP_PROG" $WRAP_ARGS "$LC"
            else
                set -- "$@" "$LC"
            fi
            rotated=1
        else
            set -- "$@" "$a"
        fi
    done
    set -- env -i PATH="$PATH" HOME="$TMP/home" \
        XDG_DATA_HOME="$TMP/home/.local/share" \
        XDG_CACHE_HOME="$TMP/cache" CC="$E2E_CC" CXX="$E2E_CXX" \
        TERM=xterm LC_ALL=C COLUMNS=80 LC_PROBLEMS="$FIX" \
        ASAN_OPTIONS="${ASAN_OPTIONS-}" UBSAN_OPTIONS="${UBSAN_OPTIONS-}" \
        "$@"
    if [ -n "${PTY_COLS-}" ]; then
        set -- "$PYTHON" "$TMP/lcpty.py" "$@"
    fi
    "$@"
}

# run [VAR=value...] -- ARGS...: stdout in $TMP/out, stderr in $TMP/err,
# exit status in $st.
run() {
    lc_env "$@" >"$TMP/out" 2>"$TMP/err" </dev/null
    st=$?
}

# [VAR=value...] -- ARGS... as a command line, for messages.
describe() {
    d=
    seen_lc=
    for a do
        if [ "x$a" = x-- ] && [ -z "$seen_lc" ]; then
            d="${d}lc"
            seen_lc=1
        else
            d="$d$a"
        fi
        d="$d "
    done
    printf '%s' "${d% }"
}

# 0 if the lc arguments ask for JSON: a --json before lc's own "--".
json_wanted() {
    seen=
    for a do
        if [ -z "$seen" ]; then
            [ "x$a" = x-- ] && seen=1
            continue
        fi
        [ "x$a" = x-- ] && return 1
        [ "x$a" = x--json ] && return 0
    done
    return 1
}

# stdout must be exactly one line of valid JSON.
check_json() {
    n=$(wc -l <"$TMP/out" | tr -d ' ')
    last=$(tail -c 1 "$TMP/out" | wc -l | tr -d ' ')
    if [ "$n" != 1 ] || [ "$last" != 1 ]; then
        bad "$1" "JSON output is not exactly one line"
        return
    fi
    if [ -n "$PYTHON" ] && ! "$PYTHON" -c '
import json, sys
json.loads(sys.stdin.buffer.read().decode("utf-8"))' <"$TMP/out" 2>/dev/null
    then
        bad "$1" "stdout is not valid JSON"
    fi
}

record() {
    echo "--- stdout"
    normalize <"$TMP/out"
    echo "--- stderr"
    normalize <"$TMP/err"
    echo "--- exit $st"
}

# compare NAME DESC MODE: MODE "golden" may rewrite the file under UPDATE=1,
# "same" only compares with a golden file written by an earlier test.
compare() {
    exp=$EXP/$1.txt
    if [ -n "$UPDATE" ] && [ "$3" = golden ]; then
        if ! cmp -s "$TMP/got" "$exp"; then
            cp "$TMP/got" "$exp"
            updated=$((updated + 1))
        fi
        ok
        return
    fi
    if [ ! -f "$exp" ]; then
        bad "$2" "no golden file $exp (run with UPDATE=1)"
    elif cmp -s "$TMP/got" "$exp"; then
        ok
    else
        bad "$2" "output differs from $exp"
        diff -u "$exp" "$TMP/got" | head -n 40
    fi
}

# run_compare MODE NAME [VAR=value...] -- ARGS...
run_compare() {
    mode=$1
    name=$2
    shift 2
    desc=$(describe "$@")
    run "$@"
    record >"$TMP/got"
    if json_wanted "$@"; then
        check_json "$desc"
    fi
    compare "$name" "$desc" "$mode"
}

# check NAME [VAR=value...] -- ARGS...: runs lc and compares the result with
# tests/expected/NAME.txt.
check() {
    run_compare golden "$@"
}

# same NAME [VAR=value...] -- ARGS...: like check, but the output must equal
# that of the earlier test that wrote NAME.
same() {
    run_compare same "$@"
}

# expect_clean DESC: the last run exited 0 with nothing on stderr.
expect_clean() {
    if [ "$st" != 0 ]; then
        bad "$1" "exit status $st, expected 0"
    elif [ -s "$TMP/err" ]; then
        bad "$1" "unexpected stderr: $(head -n 3 "$TMP/err")"
    else
        ok
    fi
}

# ------------------------------------------------------------------
# lc list
# ------------------------------------------------------------------

check list -- list
check list-json -- list --json
check list-easy -- list --difficulty easy
check list-easy-hard -- list --difficulty easy,hard
same list-easy-hard -- list --difficulty=Hard,EASY
same list-easy-hard -- list --difficulty hard --difficulty easy
same list-easy-hard -- list --difficulty 'easy, hard'
check list-tag -- list --tag hash-table
check list-tags -- list --tag array --tag hash-table
same list-tags -- list --tag=hash-table --tag array
same list-tags -- list --tag array --tag hash-table --
check list-tag-difficulty-json -- list --tag array --difficulty hard --json
check list-none -- list --tag no-such-tag
check list-none-json -- list --difficulty medium --tag array --json
check list-color -- list --color=always
same list-color NO_COLOR=1 -- list --color always
same list -- list --color never
same list -- list --color=auto
same list NO_COLOR=1 -- list
check list-bad LC_PROBLEMS=tests/fixtures/bad -- list
check list-bad-json LC_PROBLEMS=tests/fixtures/bad -- list --json
check list-cli LC_PROBLEMS=$CLIFIX -- list
check list-empty-dir LC_PROBLEMS="$TMP/empty" -- list
check list-empty-dir-json LC_PROBLEMS="$TMP/empty" -- list --json

# ------------------------------------------------------------------
# lc show
# ------------------------------------------------------------------

check show-1 -- show 1
same show-1 -- show two-sum
same show-1 -- show 0001
same show-1 -- show 0001-two-sum
same show-1 -- show 1 --no-pager
same show-1 -- show 1 --color=auto
same show-1 PAGER=false -- show 1
same show-1 COLUMNS= -- show 1
same show-1 COLUMNS=abc -- show 1
same show-1 COLUMNS=0 -- show 1
check show-1-json -- show 1 --json
check show-1-color -- show 1 --color=always
same show-1-color NO_COLOR=1 -- show 1 --color always
check show-4 -- show 4
check show-141 -- show 141
check show-141-json -- show linked-list-cycle --json
check show-155 -- show 155
check show-650 -- show 650
check show-650-json -- show 650 --json
check show-2667 -- show 2667
check show-2667-json -- show 2667 --json
check show-sink LC_PROBLEMS=$CLIFIX -- show kitchen-sink
check show-sink-json LC_PROBLEMS=$CLIFIX -- show 1 --json
check show-sink-color LC_PROBLEMS=$CLIFIX -- show 1 --color=always
check show-sink-40 COLUMNS=40 LC_PROBLEMS=$CLIFIX -- show 1
check show-sink-20 COLUMNS=20 LC_PROBLEMS=$CLIFIX -- show 1
same show-sink-20 COLUMNS=5 LC_PROBLEMS=$CLIFIX -- show 1
check show-sink-100 COLUMNS=100 LC_PROBLEMS=$CLIFIX -- show 1
same show-sink-100 COLUMNS=1000 LC_PROBLEMS=$CLIFIX -- show 1
check show-broken-tests LC_PROBLEMS=$CLIFIX -- show 2
check show-broken-tests-json LC_PROBLEMS=$CLIFIX -- show broken-tests --json
check show-no-signature LC_PROBLEMS=$CLIFIX -- show 3
check show-no-signature-json LC_PROBLEMS=$CLIFIX -- show 3 --json
check show-bad LC_PROBLEMS=tests/fixtures/bad -- show alpha
check show-bad-json LC_PROBLEMS=tests/fixtures/bad -- show 10000 --json

# ------------------------------------------------------------------
# Option order and "--"
# ------------------------------------------------------------------

same show-1-json -- show --json 1
same show-1-json -- show --json --no-pager two-sum --color=never
same show-1 -- show -- 1
same show-1 -- show --no-pager -- 1
same show-1-color -- show --color always 1
check show-dashdash-json -- show -- --json
check err-list-dashdash -- list -- --json

# ------------------------------------------------------------------
# help and version
# ------------------------------------------------------------------

check help -- help
same help -- --help
same help -- -h
check help-json -- help --json
same help-json -- --help --json
check help-list -- help list
same help-list -- list --help
same help-list -- list -h
same help-list -- --help list
same help-list -- list --bogus --help
check help-list-json -- help list --json
same help-list-json -- list --json --help
check help-show -- help show
same help-show -- show --help
same help-show -- show 1 -h
check help-help -- help help
same help-help -- help --help
check help-version -- help version
same help-version -- version --help
check version -- version
same version -- --version
check version-json -- version --json
same version-json -- --version --json
check no-args --

# ------------------------------------------------------------------
# Usage errors (exit status 3)
# ------------------------------------------------------------------

check err-unknown-command -- bogus
check err-unknown-command-json -- bogus --json
check err-unknown-option -- --bogus
check err-option-before-command -- --json list
check err-help-value -- --help=x
same err-help-value -- --help= list
check err-help-value-json -- --help=1 --json
check err-version-value -- --version=1
check err-h-value -- -h=1
check err-list-help-value -- list --help=1
check err-list-unknown-option -- list --bogus
check err-list-unknown-option-json -- list --bogus=1 --json
check err-list-no-pager -- list --no-pager
check err-list-extra -- list two-sum
check err-list-tag-missing -- list --tag
check err-list-tag-empty -- list --tag=
check err-list-difficulty-missing -- list --difficulty
check err-list-difficulty-unknown -- list --difficulty eazy
check err-list-difficulty-unknown-json -- list --json --difficulty=easy,expert
check err-list-difficulty-empty -- list --difficulty easy,,hard
check err-list-difficulty-trailing -- list --difficulty easy,
check err-color -- list --color=rainbow
check err-color-missing -- show 1 --color
check err-json-value -- list --json=yes
check err-show-missing -- show
check err-show-missing-json -- show --json
check err-show-extra -- show 1 2
check err-show-extra-dashdash -- show -- 1 --json
check err-show-unknown-option -- show 1 -x
check err-show-color-value -- show 1 --color=sometimes --json
check err-version-extra -- version 1
check err-version-color -- version --color=always
check err-help-unknown -- help bogus
check err-help-extra -- help list show
check err-help-no-pager -- help --no-pager

# ------------------------------------------------------------------
# Unknown problems and load failures
# ------------------------------------------------------------------

check err-show-unknown -- show 999
check err-show-unknown-json -- show nope --json
check err-show-wrong-slug -- show 1-three-sum
check err-show-zero -- show 0
check err-missing-dir LC_PROBLEMS="$TMP/missing" -- list
check err-missing-dir-json LC_PROBLEMS="$TMP/missing" -- show 1 --json
check err-not-dir LC_PROBLEMS=tests/e2e.sh -- list

# No packs anywhere: no LC_PROBLEMS and an empty home. The installed
# location ($(PREFIX)/share/lcclone/problems) may exist on this machine.
run LC_PROBLEMS= -- list
if [ "$st" = 0 ]; then
    echo "note: skipped 'no packs found' (packs are installed on this machine)"
else
    if [ "$st" = 3 ] && normalize <"$TMP/err" | grep -q \
        "^lc: no problem directory found (tried '@TMP@/home/.local/share/lcclone/problems'"
    then
        ok
    else
        bad "lc list (no packs anywhere)" "exit $st, stderr: $(cat "$TMP/err")"
    fi
    run LC_PROBLEMS= -- list --json
    check_json "lc list --json (no packs anywhere)"
    if [ "$st" = 3 ] &&
        grep -q '"error":{"message":"no problem directory found' "$TMP/out"
    then
        ok
    else
        bad "lc list --json (no packs anywhere)" "exit $st, stdout: $(cat "$TMP/out")"
    fi
fi

# Without LC_PROBLEMS, $XDG_DATA_HOME/lcclone/problems comes before
# ~/.local/share/lcclone/problems, as 'lc help' says.
mkdir -p "$TMP/xdg/lcclone"
ln -s "$ROOT/$FIX" "$TMP/xdg/lcclone/problems"
same list LC_PROBLEMS= XDG_DATA_HOME="$TMP/xdg" -- list

# ------------------------------------------------------------------
# A closed pipe: "lc list | head -1" prints one line, exits 0, no error
# ------------------------------------------------------------------

# pipe_head DESC [VAR=value...] -- ARGS...
pipe_head() {
    d=$1
    shift
    { lc_env "$@" 2>"$TMP/err" </dev/null; echo $? >"$TMP/st"; } |
        head -n 1 >"$TMP/out"
    st=$(cat "$TMP/st")
    n=$(wc -l <"$TMP/out" | tr -d ' ')
    if [ "$st" != 0 ]; then
        bad "$d" "exit status $st, expected 0"
    elif [ -s "$TMP/err" ]; then
        bad "$d" "unexpected stderr: $(head -n 3 "$TMP/err")"
    elif [ "$n" != 1 ]; then
        bad "$d" "expected one line, got $n"
    else
        ok
    fi
}

pipe_head "lc list | head -1" -- list

# Outputs much larger than a pipe buffer, so that lc really sees EPIPE.
mkdir "$TMP/big" "$TMP/many"
mkdir "$TMP/big/0001-big"
awk 'BEGIN {
    printf "id: 1\nslug: big\ntitle: Big\ndifficulty: easy\n---\n"
    for (i = 0; i < 4000; i++)
        printf("Paragraph %d has enough words in it to make the output big.\n\n", i)
}' >"$TMP/big/0001-big/problem.md"
pipe_head "lc show 1 | head -1 (large statement)" LC_PROBLEMS="$TMP/big" -- show 1
# Relative names, so that blanks in $TMP don't matter to xargs or awk.
(
    cd "$TMP/many" || exit 1
    awk 'BEGIN { for (i = 1; i <= 3000; i++) printf("%04d-problem-%d\n", i, i) }' |
        xargs mkdir
    awk 'BEGIN {
        for (i = 1; i <= 3000; i++) {
            f = sprintf("%04d-problem-%d/problem.md", i, i)
            printf("id: %d\nslug: problem-%d\ntitle: Problem number %d\n", i, i, i) > f
            printf("difficulty: easy\ntags: array, string\n---\nx\n") > f
            close(f)
        }
    }'
)
pipe_head "lc list | head -1 (3000 problems)" LC_PROBLEMS="$TMP/many" -- list
pipe_head "lc list --json | head -1 (3000 problems)" LC_PROBLEMS="$TMP/many" -- list --json

# ------------------------------------------------------------------
# The repository's own problem packs load without warnings
# ------------------------------------------------------------------

if [ -d problems ]; then
    run LC_PROBLEMS=problems -- list
    expect_clean "LC_PROBLEMS=problems lc list"
    if [ ! -s "$TMP/out" ]; then
        bad "LC_PROBLEMS=problems lc list" "no problems listed"
    fi
    for id in $(awk 'NR > 1 { print $1 }' "$TMP/out"); do
        run LC_PROBLEMS=problems -- show "$id"
        expect_clean "LC_PROBLEMS=problems lc show $id"
        run LC_PROBLEMS=problems -- show "$id" --json
        expect_clean "LC_PROBLEMS=problems lc show $id --json"
        check_json "LC_PROBLEMS=problems lc show $id --json"
    done
    run LC_PROBLEMS=problems -- list --json
    expect_clean "LC_PROBLEMS=problems lc list --json"
    check_json "LC_PROBLEMS=problems lc list --json"
fi

# ------------------------------------------------------------------
# lc start: setting up a problem's directory
# ------------------------------------------------------------------

WS=$TMP/ws
JFIX=tests/fixtures/judge
J=$JFIX/0010-add

# files_golden NAME DIR FILE...: the files, one after another, must match
# tests/expected/NAME.txt.
files_golden() {
    fg_name=$1
    fg_dir=$2
    shift 2
    for f do
        echo "--- $f"
        if [ -f "$fg_dir/$f" ]; then
            normalize <"$fg_dir/$f"
        else
            echo "(missing)"
        fi
    done >"$TMP/got"
    compare "$fg_name" "files of $fg_dir" golden
}

check start-1 LC_WORKSPACE="$WS" LC_PROBLEMS=problems -- start 1 --no-edit
files_golden start-1-files "$WS" 0001-two-sum/README.md \
    0001-two-sum/solution.cpp 0001-two-sum/cases.txt .clangd
if cmp -s "$WS/.lcclone/lc_prelude.hpp" runtime/cpp/lc_prelude.hpp &&
    cmp -s "$WS/.lcclone/lc_prelude.h" runtime/c/lc_prelude.h; then
    ok
else
    bad "lc start 1" "the preludes in .lcclone are not the runtime's"
fi
check start-1-c LC_WORKSPACE="$WS" LC_PROBLEMS=problems -- start 1 --lang c --no-edit
files_golden start-1-c-files "$WS" 0001-two-sum/solution.c
check start-867 LC_WORKSPACE="$WS" LC_PROBLEMS=problems -- start transpose-matrix --lang=c --no-edit
files_golden start-867-files "$WS" 0867-transpose-matrix/README.md \
    0867-transpose-matrix/solution.c 0867-transpose-matrix/cases.txt
check start-104 LC_WORKSPACE="$WS" LC_PROBLEMS=problems -- start 104 --no-edit --json
files_golden start-104-files "$WS" 0104-maximum-depth-of-binary-tree/solution.cpp

# Your files are never overwritten; README.md is rewritten.
echo '// mine' >"$WS/0001-two-sum/solution.cpp"
echo '# mine' >"$WS/0001-two-sum/cases.txt"
echo 'stale' >"$WS/0001-two-sum/README.md"
check start-1-again-json LC_WORKSPACE="$WS" LC_PROBLEMS=problems -- start 1 --no-edit --json
if [ "$(cat "$WS/0001-two-sum/solution.cpp")" = '// mine' ] &&
    [ "$(cat "$WS/0001-two-sum/cases.txt")" = '# mine' ] &&
    ! grep -q stale "$WS/0001-two-sum/README.md"; then
    ok
else
    bad "lc start 1 (again)" "overwrote a file of yours, or not README.md"
fi
# With both solutions there, the default is C++; with only a C one, C.
mkdir -p "$TMP/ws2/0009-palindrome-number"
echo 'bool isPalindrome(int x) { return x == 0; }' \
    >"$TMP/ws2/0009-palindrome-number/solution.c"
check start-existing-c LC_WORKSPACE="$TMP/ws2" LC_PROBLEMS=problems -- start 9 --no-edit

# A .clangd that lc didn't write is left alone.
printf 'CompileFlags:\n  Add: [-Wall]\n' >"$TMP/ws2/.clangd"
check start-own-clangd LC_WORKSPACE="$TMP/ws2" LC_PROBLEMS=problems -- start 9 --no-edit
if [ "$(head -n 1 "$TMP/ws2/.clangd")" = 'CompileFlags:' ]; then
    ok
else
    bad "lc start 9 (own .clangd)" ".clangd was overwritten"
fi
# $LC_CXXSTD reaches .clangd.
check start-cxxstd LC_WORKSPACE="$TMP/ws3" LC_CXXSTD=c++17 LC_PROBLEMS=problems -- start 20 --no-edit
files_golden start-cxxstd-files "$TMP/ws3" .clangd

check err-start-missing -- start
check err-start-lang -- start 1 --lang rust
check err-start-no-signature LC_WORKSPACE="$WS" LC_PROBLEMS=$CLIFIX -- start 3 --no-edit
check err-start-unknown -- start 999 --no-edit
check err-start-no-home LC_WORKSPACE= HOME= -- start 1 --no-edit --json
check help-start -- help start

# The editor: $VISUAL, else $EDITOR (with arguments), run in the problem's
# directory on the solution file.
cat >"$TMP/ed.sh" <<'EOF'
#!/bin/sh
{ pwd; printf '%s\n' "$@"; } >"$ED_LOG"
exit ${ED_STATUS:-0}
EOF
chmod +x "$TMP/ed.sh"
ed_check() {  # ed_check DESC STATUS EXPECTED-LOG: the last run's outcome
    if [ "$st" != "$2" ]; then
        bad "$1" "exit status $st, expected $2: $(cat "$TMP/err")"
    elif [ "$(cat "$TMP/edlog" 2>/dev/null)" != "$3" ]; then
        bad "$1" "the editor got: $(cat "$TMP/edlog" 2>/dev/null)"
    else
        ok
    fi
    rm -f "$TMP/edlog"
}
run LC_WORKSPACE="$WS" LC_PROBLEMS=problems VISUAL="$TMP/ed.sh -x" \
    EDITOR=no-such-editor ED_LOG="$TMP/edlog" -- start 1
ed_check "VISUAL=ed.sh lc start 1" 0 "$(cd "$WS/0001-two-sum" && pwd -P)
-x
solution.cpp"
run LC_WORKSPACE="$WS" LC_PROBLEMS=problems EDITOR="$TMP/ed.sh" \
    ED_LOG="$TMP/edlog" -- start 1 --lang c
ed_check "EDITOR=ed.sh lc start 1 --lang c" 0 "$(cd "$WS/0001-two-sum" && pwd -P)
solution.c"
run LC_WORKSPACE="$WS" LC_PROBLEMS=problems EDITOR="$TMP/ed.sh" \
    ED_LOG="$TMP/edlog" ED_STATUS=4 -- start 1
ed_check "lc start 1 (the editor fails)" 0 "$(cd "$WS/0001-two-sum" && pwd -P)
solution.cpp"
if ! grep -q "^lc: warning: the editor '.*ed.sh' exited with status 4" "$TMP/err"; then
    bad "lc start 1 (the editor fails)" "no warning: $(cat "$TMP/err")"
fi
run LC_WORKSPACE="$WS" LC_PROBLEMS=problems VISUAL=no-such-editor -- start 1
ed_check "VISUAL=no-such-editor lc start 1" 3 ""

# ------------------------------------------------------------------
# lc test and lc submit
# ------------------------------------------------------------------

check test-add LC_PROBLEMS=$JFIX -- test $J/solution.c
check submit-add-cpp LC_PROBLEMS=$JFIX -- submit $J/solution.cpp
check test-add-wrong LC_PROBLEMS=$JFIX -- test $J/wrong.c
check test-add-chatty LC_PROBLEMS=$JFIX -- test $J/chatty.c
check submit-add-crash LC_PROBLEMS=$JFIX -- submit $J/crash.c
check submit-add-exit1 LC_PROBLEMS=$JFIX -- submit $J/exit1.c
check submit-add-flood LC_PROBLEMS=$JFIX -- submit $J/flood.c
check test-add-mycases LC_PROBLEMS=$JFIX -- test $J/wrong.c --case $J/mycases.txt
check submit-reverse-unset LC_PROBLEMS=$JFIX -- submit $JFIX/0011-reverse/unset.c
check submit-reverse-null LC_PROBLEMS=$JFIX -- submit $JFIX/0011-reverse/null.c
check err-test-badcase LC_PROBLEMS=$JFIX -- test $J/solution.c --case $J/badcase.txt
check err-test-no-ref LC_PROBLEMS=$JFIX -- test $JFIX/0011-reverse/solution.c
check err-test-bad-ref LC_PROBLEMS=$JFIX -- test $JFIX/0012-bad-ref/solution.c
check err-test-tree LC_PROBLEMS=$JFIX -- test $JFIX/0013-tree/problem.md --lang c
check err-test-std LC_PROBLEMS=$JFIX -- test $J/solution.c --std c++17
check err-test-std-env LC_CXXSTD=gnu++17 LC_PROBLEMS=$JFIX -- test $J/solution.cpp
check err-test-case-missing LC_PROBLEMS=$JFIX -- test $J/solution.c --case $J/nope.txt
check help-test -- help test
check help-submit -- help submit

# expect_run DESC STATUS PATTERN... [VAR=value...] -- ARGS: lc exits with
# STATUS, and its stdout has a line matching each grep PATTERN (for output
# that differs between compilers).
expect_run() {
    er_desc=$1
    er_st=$2
    shift 2
    er_pats=
    while [ $# -gt 0 ] && [ "$1" != -- ] && [ "${1#*=}" = "$1" ]; do
        er_pats="$er_pats
$1"
        shift
    done
    run LC_PROBLEMS=$JFIX "$@"
    if [ "$st" != "$er_st" ]; then
        bad "$er_desc" "exit status $st, expected $er_st: $(head -n 5 "$TMP/err") $(head -n 5 "$TMP/out")"
        return
    fi
    echo "$er_pats" | while IFS= read -r pat; do
        [ -z "$pat" ] && continue
        grep -q -e "$pat" "$TMP/out" || echo "$pat"
    done >"$TMP/missing"
    if [ -s "$TMP/missing" ]; then
        bad "$er_desc" "no line matches '$(head -n 1 "$TMP/missing")' in: $(head -n 12 "$TMP/out")"
    else
        ok
    fi
}

D=tests/fixtures/judge/0010-add
expect_run "lc submit spin.c" 1 '^Time Limit Exceeded: 2/5 cases passed$' \
    'spin.c:1: Time Limit Exceeded on case 3 of 5' -- submit $J/spin.c
expect_run "lc submit alloc.c" 1 '^Memory Limit Exceeded: 2/5 cases passed$' \
    -- submit $J/alloc.c
expect_run "lc submit throw.cpp" 1 \
    'throw.cpp:1: Runtime Error on case 3 of 5 (killed by SIGABRT)' \
    'no hundreds' -- submit $J/throw.cpp
expect_run "lc test ce.c" 2 '^Building .*ce.c (C, c11)$' \
    "^$D/ce.c:2:[0-9]*: error: " '^Compile Error$' -- test $J/ce.c
# clang points at the line missing its ';', g++ at the next one.
expect_run "lc test ce.cpp" 2 "^$D/ce.cpp:[45]:[0-9]*: error: " \
    '^lc: this was built as C++11' -- test $J/ce.cpp
expect_run "lc test sig.c" 2 '^lc_harness:[0-9]*:[0-9]*: error: ' \
    '^lc: errors in lc_harness' '^    int add(int a, int b)$' \
    -- test $J/sig.c
expect_run "lc test sig.cpp" 2 '^    int Solution::add(int a, int b)$' \
    -- test $J/sig.cpp
expect_run "lc test warn.c" 0 "^$D/warn.c:4:[0-9]*: warning: " \
    '^Accepted: 2/2 cases passed' -- test $J/warn.c
expect_run "lc test cxx14.cpp" 2 '^lc: this was built as C++11' \
    -- test $J/cxx14.cpp
expect_run "lc test cxx14.cpp --std c++14" 0 '(C++, c++14)$' \
    -- test $J/cxx14.cpp --std c++14
expect_run "lc test cxx17.cpp --std=max" 0 '^Accepted' \
    -- test $J/cxx17.cpp --std=max
expect_run "LC_CXXSTD=c++17 lc test cxx17.cpp" 0 '(C++, c++17)$' \
    LC_CXXSTD=c++17 -- test $J/cxx17.cpp
expect_run "LC_CXXSTD=c++17 lc test cxx17.cpp --std c++14" 2 \
    '(C++, c++14)$' LC_CXXSTD=c++17 -- test $J/cxx17.cpp --std c++14
expect_run "LC_CSTD=c17 lc test solution.c" 0 '(C, c17)$' \
    LC_CSTD=c17 -- test $J/solution.c
expect_run "CXX=no-such-compiler lc test" 3 CXX=no-such-compiler \
    -- test $J/solution.cpp
if ! grep -q "^lc: the C++ compiler 'no-such-compiler' (from \$CXX) was not found$" "$TMP/err"; then
    bad "CXX=no-such-compiler lc test" "stderr: $(cat "$TMP/err")"
fi

# Which solution: a file, a directory, a problem, or the current directory.
expect_run "lc test DIR with two solutions" 3 -- test $J
grep -q "^lc: $J has both a C and a C++ solution; choose one with --lang c or --lang cpp$" "$TMP/err" ||
    bad "lc test DIR with two solutions" "stderr: $(cat "$TMP/err")"
expect_run "lc test DIR --lang cpp" 0 "^Building $D/solution.cpp " -- test $J --lang cpp
# in_dir DIR [VAR=value...] -- ARGS: run in DIR.
in_dir() {
    id_dir=$1
    shift
    (cd "$id_dir" && lc_env "$@") >"$TMP/out" 2>"$TMP/err" </dev/null
    st=$?
}
in_dir "$J" LC_PROBLEMS="$ROOT/$JFIX" -- test --lang c
if [ "$st" = 0 ] && grep -q '^Building solution.c (C, c11)$' "$TMP/out"; then
    ok
else
    bad "lc test --lang c (in the problem's directory)" "exit $st: $(cat "$TMP/out" "$TMP/err")"
fi
in_dir "$TMP" LC_PROBLEMS="$ROOT/$JFIX" -- test
if [ "$st" = 3 ] && grep -q "^lc: no problem given, and the current directory is not a problem directory" "$TMP/err"; then
    ok
else
    bad "lc test (outside a problem directory)" "exit $st: $(cat "$TMP/err")"
fi
check err-test-not-found LC_PROBLEMS=$JFIX -- test nosuch.c
check err-test-not-problem-dir LC_PROBLEMS=$JFIX -- test tests/e2e.sh --lang c
check err-test-extension LC_PROBLEMS=$JFIX -- test $J/problem.md
check err-test-no-workspace-dir LC_WORKSPACE="$TMP/ws9" LC_PROBLEMS=$JFIX -- test add
# The workspace: lc start, then lc test and submit by ID or slug.
run LC_WORKSPACE="$TMP/ws4" LC_PROBLEMS=$JFIX -- start 10 --lang c --no-edit
cp $J/solution.c "$TMP/ws4/0010-add/solution.c"
check test-workspace LC_WORKSPACE="$TMP/ws4" LC_PROBLEMS=$JFIX -- test 10
check submit-workspace LC_WORKSPACE="$TMP/ws4" LC_PROBLEMS=$JFIX -- submit add
# cases.txt next to the solution is run too (its width depends on $TMP, so
# no golden file).
printf '7\n8\n' >>"$TMP/ws4/0010-add/cases.txt"
expect_run "lc test 10 (with cases.txt)" 0 '^Building the reference solution' \
    '/ws4/0010-add/cases.txt:18  *Accepted' '^Accepted: 3/3 cases passed' \
    LC_WORKSPACE="$TMP/ws4" -- test 10
check err-test-no-solution LC_WORKSPACE="$TMP/ws4" LC_PROBLEMS=$JFIX -- test 10 --lang cpp

# JSON Lines: every line is an object with "schema" and "event", in order.
json_events() {  # json_events DESC STATUS EVENTS: the last run's events
    if [ "$st" != "$2" ]; then
        bad "$1" "exit status $st, expected $2: $(cat "$TMP/err")"
        return
    fi
    [ -n "$PYTHON" ] || { ok; return; }
    if got=$("$PYTHON" -c '
import json, sys
evs = []
for line in sys.stdin.buffer.read().decode("utf-8").splitlines():
    d = json.loads(line)
    assert d["schema"] == 1, line
    if "error" in d:
        evs.append("error")
        continue
    e = d["event"]
    if e in ("build_start", "build_done"):
        e += "(ref)" if d["reference"] else ""
        if e.startswith("build_done") and not d["ok"]:
            assert d["diagnostics"], line
            e += "!"
    if e == "case_done":
        e += ":" + d["verdict"]
    if e == "done":
        e += ":%s:%d/%d" % (d["verdict"], d["passed"], d["total"])
        assert isinstance(d["warnings"], list)
    evs.append(e)
print(" ".join(evs))' <"$TMP/out" 2>&1) && [ "$got" = "$3" ]; then
        ok
    else
        bad "$1" "events: $got"
    fi
}
run LC_PROBLEMS=$JFIX -- test $J/wrong.c --case $J/mycases.txt --json
json_events "lc test wrong.c --case mycases.txt --json" 1 "build_start build_done build_start(ref) build_done(ref) case_start case_done:accepted case_start case_done:wrong_answer case_start case_done:accepted case_start case_done:accepted case_start case_done:wrong_answer done:wrong_answer:3/5"
run LC_PROBLEMS=$JFIX -- submit $J/crash.c --json
json_events "lc submit crash.c --json" 1 "build_start build_done case_start case_done:accepted case_start case_done:accepted case_start case_done:runtime_error done:runtime_error:2/5"
run LC_PROBLEMS=$JFIX -- submit $J/ce.c --json
json_events "lc submit ce.c --json" 2 "build_start build_done! done:compile_error:0/5"
run LC_PROBLEMS=$JFIX -- submit $J/solution.c --std c++17 --json
json_events "lc submit --std c++17 --json" 3 "error"
run LC_PROBLEMS=$JFIX -- test $J/chatty.c --json
check_json_lines() {
    [ -n "$PYTHON" ] || return 0
    if "$PYTHON" -c '
import json, sys
for line in sys.stdin.buffer.read().decode("utf-8").splitlines():
    d = json.loads(line)
    if d.get("event") == "case_done":
        assert d["stdout"] == "a=1\n" or d["stdout"].startswith("a="), d
        assert d["stderr"].startswith("b="), d
        assert d["inputs"][0]["name"] == "a", d
        assert d["exit_code"] == 0 and d["signal"] is None, d
        assert d["file"].endswith("/tests.txt"), d
' <"$TMP/out"; then
        ok
    else
        bad "lc test chatty.c --json" "unexpected case_done"
    fi
}
check_json_lines

# Vim's default 'errorformat' finds the failed cases and compile errors.
VIM=$(command -v vim 2>/dev/null || command -v nvim 2>/dev/null)
qf_check() {  # qf_check DESC EXPECTED: the valid quickfix entries of $TMP/out
    "$VIM" -es -u NONE -i NONE -N -c 'set efm&' -c "cgetfile $TMP/out" \
        -c "redir! > $TMP/qf" \
        -c 'for e in getqflist() | if e.valid | echo bufname(e.bufnr) . ":" . e.lnum | endif | endfor' \
        -c 'redir END' -c 'qa!' </dev/null >/dev/null 2>&1
    got=$(grep . "$TMP/qf" 2>/dev/null | tr '\n' ' ')
    if [ "$got" = "$2" ]; then
        ok
    else
        bad "$1" "quickfix entries: '$got', expected '$2'"
    fi
}
if [ -n "$VIM" ]; then
    run LC_PROBLEMS=$JFIX -- test $J/wrong.c --case $J/mycases.txt
    qf_check "lc test wrong.c in Vim's quickfix list" "$D/wrong.c:1 $D/mycases.txt:9 "
    run LC_PROBLEMS=$JFIX -- test $J/ce.c
    qf_check "lc test ce.c in Vim's quickfix list" "$D/ce.c:2 "
else
    echo "note: skipped the quickfix tests (no vim or nvim)"
fi

# --sanitize, where the C compiler has the sanitizers (musl's gcc doesn't).
printf 'int main(void) { return 0; }\n' >"$TMP/san.c"
# shellcheck disable=SC2086 # CC may be several words
if $E2E_CC -fsanitize=address,undefined -o "$TMP/san" "$TMP/san.c" >/dev/null 2>&1 &&
    "$TMP/san"; then
    expect_run "lc submit overflow.c --sanitize" 1 \
        '(C, c11, sanitizers)$' 'overflow.c:1: Runtime Error on case 3 of 5' \
        -- submit $J/overflow.c --sanitize
    expect_run "lc submit overflow.c" 0 '^Accepted' -- submit $J/overflow.c
else
    echo "note: skipped the --sanitize tests ($E2E_CC has no sanitizers)"
fi

# ^C stops a run: lc kills the program and exits with status 130.
if [ -z "$WRAP_PROG" ]; then
    env -i PATH="$PATH" HOME="$TMP/home" XDG_CACHE_HOME="$TMP/cache" \
        CC="$E2E_CC" CXX="$E2E_CXX" LC_ALL=C LC_PROBLEMS=$JFIX \
        ASAN_OPTIONS="${ASAN_OPTIONS-}" "$LC" submit $J/spin.c \
        >"$TMP/out" 2>"$TMP/err" </dev/null &
    pid=$!
    # Long enough to be past the build (cached by now) and into case 3.
    sleep 1
    kill -INT "$pid" 2>/dev/null
    wait "$pid"
    st=$?
    if [ "$st" = 130 ] && grep -q '^Cancelled after 2 of 5 cases$' "$TMP/out"; then
        ok
    else
        bad "^C during lc submit spin.c" "exit $st: $(cat "$TMP/out" "$TMP/err")"
    fi
fi

# Every shipped problem: its reference solution in C, and the C++
# solution in tests/fixtures/solutions at c++11 and at the newest
# standard, pass every case.
for d in problems/*/; do
    b=$(basename "$d")
    if [ -f "$d/ref.c" ]; then
        mkdir -p "$TMP/shipped/$b"
        cp "$d/ref.c" "$TMP/shipped/$b/solution.c"
        run LC_PROBLEMS=problems -- submit "$TMP/shipped/$b/solution.c"
        if [ "$st" = 0 ]; then ok; else
            bad "lc submit (problem $b, ref.c)" "$(tail -n 5 "$TMP/out") $(cat "$TMP/err")"
        fi
    fi
    if [ -f "tests/fixtures/solutions/$b.cpp" ]; then
        mkdir -p "$TMP/shipped/$b"
        cp "tests/fixtures/solutions/$b.cpp" "$TMP/shipped/$b/solution.cpp"
        for std in c++11 max; do
            run LC_PROBLEMS=problems -- submit "$TMP/shipped/$b/solution.cpp" --std $std
            if [ "$st" = 0 ]; then ok; else
                bad "lc submit (problem $b, C++, $std)" "$(tail -n 5 "$TMP/out") $(cat "$TMP/err")"
            fi
        done
    fi
done

# ------------------------------------------------------------------
# Running out of memory
# ------------------------------------------------------------------

# An allocator to preload: LC_FAIL_AT=N makes allocation N fail, and with
# LC_FAIL_ALL=1 every later one too. The first failure creates the file
# $LC_FAIL_MARK, so a run without it had nothing left to fail.
cat >"$TMP/failmalloc.c" <<'EOF'
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define POOL_SIZE ((size_t)64 << 20)
#define HDR 16 /* room for the block's size; also the least alignment */

void *memalign(size_t align, size_t size);
size_t malloc_usable_size(void *p);

/* Memory is never reused, so it is always zero. */
static union {
    max_align_t align;
    unsigned char b[POOL_SIZE];
} pool;
static size_t used;
static unsigned long count;

static int inject(void)
{
    const char *at = getenv("LC_FAIL_AT"), *all = getenv("LC_FAIL_ALL");
    const char *mark = getenv("LC_FAIL_MARK");
    unsigned long n = 0;
    int fd;

    count++;
    for (; at && *at >= '0' && *at <= '9'; at++)
        n = n * 10 + (unsigned long)(*at - '0');
    if (n == 0 || count < n || (count > n && !(all && *all)))
        return 0;
    if (count == n && mark && *mark) {
        fd = open(mark, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0)
            close(fd);
    }
    return 1;
}

static void *take(size_t size, size_t align)
{
    size_t start;

    if (align < HDR)
        align = HDR;
    if (inject() || size > POOL_SIZE || align > POOL_SIZE / 2) {
        errno = ENOMEM;
        return NULL;
    }
    start = (used + HDR + align - 1) / align * align;
    if (start > POOL_SIZE || size > POOL_SIZE - start) {
        errno = ENOMEM;
        return NULL;
    }
    used = start + size;
    memcpy(pool.b + start - HDR, &size, sizeof size);
    return pool.b + start;
}

void *malloc(size_t size)
{
    return take(size, HDR);
}

void free(void *p)
{
    (void)p;
}

void *calloc(size_t n, size_t size)
{
    if (size && n > (size_t)-1 / size) {
        errno = ENOMEM;
        return NULL;
    }
    return take(n * size, HDR);
}

void *realloc(void *old, size_t size)
{
    void *p = take(size, HDR);

    if (p && old) {
        size_t n = malloc_usable_size(old);
        memcpy(p, old, n < size ? n : size);
    }
    return p;
}

int posix_memalign(void **out, size_t align, size_t size)
{
    void *p = take(size, align);

    if (!p)
        return ENOMEM;
    *out = p;
    return 0;
}

void *aligned_alloc(size_t align, size_t size)
{
    return take(size, align);
}

void *memalign(size_t align, size_t size)
{
    return take(size, align);
}

size_t malloc_usable_size(void *p)
{
    size_t size = 0;

    if (p)
        memcpy(&size, (unsigned char *)p - HDR, sizeof size);
    return size;
}
EOF

# oom_check FILE: sets $oom_why to what is wrong with the run whose output
# is in FILE.out and FILE.err, if anything. lc must exit 0 or 3. Without
# --json, a failure leaves stdout empty and says why on stderr. With
# --json, stdout is one line of JSON: an error document exactly when the
# status is 3, whose message is on stderr after "lc: ". python3 checks that
# in oom_report; without it only the first two points are checked here.
oom_check() {
    oom_why=
    case $st in
    0 | 3) ;;
    *)
        oom_why="exit status $st"
        return
        ;;
    esac
    if [ -z "$oom_json" ]; then
        if [ "$st" = 3 ] && [ -s "$1.out" ]; then
            oom_why="output on stdout although it failed"
        elif [ "$st" = 3 ] && ! grep -q '^lc: ' "$1.err"; then
            oom_why="no error message on stderr"
        fi
    elif [ -n "$PYTHON" ]; then
        printf '%s\t%s\t%s\n' "$oom_k" "$st" "$oom_what" >>"$TMP/oom/index"
    elif [ "$(wc -l <"$1.out" | tr -d ' ')" != 1 ] ||
        [ "$(tail -c 1 "$1.out" | wc -l | tr -d ' ')" != 1 ]; then
        oom_why="stdout is not one line"
    elif grep -q '^{"schema":1,"error":{' "$1.out"; then
        [ "$st" = 3 ] || oom_why="an error document with exit status $st"
    elif [ "$st" = 3 ]; then
        oom_why="exit status 3 without an error document"
    fi
}

# oom_sweep [VAR=value...] -- ARGS...: runs lc with allocation 1, 2, ...
# failing, first with every later allocation failing too, then alone,
# until there is no allocation left to fail.
oom_sweep() {
    oom_desc=$(describe "$@")
    oom_json=
    if json_wanted "$@"; then
        oom_json=1
    fi
    oom_bad=0
    for oom_all in 1 ''; do
        oom_n=1
        while [ "$oom_n" -le 10000 ]; do
            oom_k=$((oom_k + 1))
            oom_f=$TMP/oom/$oom_k
            lc_env LD_PRELOAD="$TMP/failmalloc.so" LC_FAIL_AT=$oom_n \
                LC_FAIL_ALL=$oom_all LC_FAIL_MARK="$oom_f.mark" "$@" \
                >"$oom_f.out" 2>"$oom_f.err" </dev/null
            st=$?
            [ -f "$oom_f.mark" ] || break
            if [ -n "$oom_all" ]; then
                oom_what="$oom_desc (allocations $oom_n and later fail)"
            else
                oom_what="$oom_desc (allocation $oom_n fails)"
            fi
            oom_check "$oom_f"
            if [ -n "$oom_why" ]; then
                oom_bad=$((oom_bad + 1))
                if [ "$oom_bad" -le 3 ]; then
                    bad "$oom_what" "$oom_why"
                else
                    fail=$((fail + 1))
                fi
            fi
            oom_n=$((oom_n + 1))
        done
    done
    if [ "$oom_bad" -gt 3 ]; then
        echo "FAIL: $oom_desc: $((oom_bad - 3)) more out-of-memory failures"
    elif [ "$oom_bad" = 0 ]; then
        ok
    fi
}

# The JSON checks of oom_check, with python3, for all the runs at once.
oom_report() {
    if [ -z "$PYTHON" ] || [ ! -s "$TMP/oom/index" ]; then
        return
    fi
    if "$PYTHON" -c '
import json, sys
d = sys.argv[1]
with open(d + "/index", "rb") as f:
    runs = f.read().decode("utf-8", "replace").splitlines()
bad = 0
for run in runs:
    k, st, what = run.split("\t", 2)
    try:
        with open("%s/%s.out" % (d, k), "rb") as f:
            out = f.read()
        with open("%s/%s.err" % (d, k), "rb") as f:
            err = f.read().decode("utf-8", "replace").splitlines()
        if out.count(b"\n") != 1 or not out.endswith(b"\n"):
            raise ValueError("stdout is not one line")
        doc = json.loads(out.decode("utf-8"))
        if doc.get("schema") != 1:
            raise ValueError("no \"schema\":1")
        if ("error" in doc) != (st == "3"):
            raise ValueError("exit status %s with %s" % (st,
                "an error document" if "error" in doc else "no error document"))
        if "error" in doc:
            msg = doc["error"]["message"]
            if not isinstance(doc.get("warnings"), list):
                raise ValueError("no warnings array")
            if "lc: " + msg not in err:
                raise ValueError("message %r is not on stderr" % msg)
    except Exception as e:
        print("FAIL: %s: %s" % (what, e))
        bad += 1
sys.exit(1 if bad else 0)' "$TMP/oom" >"$TMP/oom/report"; then
        ok
    else
        head -n 10 "$TMP/oom/report"
        fail=$((fail + $(wc -l <"$TMP/oom/report")))
    fi
}

oom_note=
mkdir "$TMP/oom"
# shellcheck disable=SC2086 # CC may be several words
if [ -n "$WRAP_PROG" ]; then
    oom_note="LC_E2E_WRAP replaces the allocator"
elif ! ${CC:-cc} -shared -fPIC -o "$TMP/failmalloc.so" "$TMP/failmalloc.c" \
    >/dev/null 2>&1; then
    oom_note="cannot build a shared library with ${CC:-cc}"
else
    run LD_PRELOAD="$TMP/failmalloc.so" LC_FAIL_AT=1 LC_FAIL_ALL=1 \
        LC_FAIL_MARK="$TMP/oom/probe.mark" -- version
    if [ "$st" != 3 ] || [ ! -f "$TMP/oom/probe.mark" ]; then
        oom_note="cannot preload an allocator into lc here"
    fi
fi
if [ -n "$oom_note" ]; then
    echo "note: skipped the out-of-memory tests ($oom_note)"
else
    oom_k=0
    oom_sweep LC_PROBLEMS=$CLIFIX -- show 1
    oom_sweep LC_PROBLEMS=$CLIFIX -- show 1 --json
    oom_sweep LC_PROBLEMS=$CLIFIX -- show broken-tests --json
    oom_sweep LC_PROBLEMS=$CLIFIX -- show 999 --json
    oom_sweep LC_PROBLEMS=$CLIFIX -- list
    oom_sweep LC_PROBLEMS=tests/fixtures/bad -- list --json
    oom_sweep -- list --bogus --json
    oom_sweep -- bogus
    oom_sweep -- bogus --json
    oom_sweep -- help --json
    oom_sweep -- version --json
    oom_sweep LC_WORKSPACE="$TMP/oomws" LC_PROBLEMS=problems -- start 1 --no-edit --json
    oom_sweep LC_WORKSPACE="$TMP/oomws" LC_PROBLEMS=problems -- start 1 --no-edit
    oom_sweep LC_PROBLEMS=$JFIX -- test $J/solution.c --std bogus --json
    oom_sweep LC_PROBLEMS=$JFIX -- test $J/solution.c --case $J/badcase.txt
    oom_report
fi

# ------------------------------------------------------------------
# On a terminal: the pager, --color=auto and the terminal's width
# ------------------------------------------------------------------

# Runs a command on a pseudo-terminal PTY_COLS columns wide, writing what
# it prints there to $PTY_OUT and its stderr to $PTY_ERR. Exits with the
# command's status, or 128 + the signal that killed it.
cat >"$TMP/lcpty.py" <<'EOF'
import errno, fcntl, os, pty, select, signal, struct, sys, termios, time

cols = int(os.environ["PTY_COLS"])
pid, master = pty.fork()
if pid == 0:
    try:
        fcntl.ioctl(1, termios.TIOCSWINSZ, struct.pack("HHHH", 24, cols, 0, 0))
        attrs = termios.tcgetattr(1)
        attrs[1] &= ~termios.OPOST  # no "\r" before each "\n"
        termios.tcsetattr(1, termios.TCSANOW, attrs)
        fd = os.open(os.environ["PTY_ERR"],
                     os.O_WRONLY | os.O_CREAT | os.O_TRUNC, 0o644)
        os.dup2(fd, 2)
        os.execvp(sys.argv[1], sys.argv[1:])
    except BaseException as e:
        os.write(2, ("lcpty.py: %s\n" % e).encode())
    os._exit(127)
data = b""
deadline = time.time() + 60
while True:
    left = deadline - time.time()
    if left <= 0:
        os.killpg(pid, signal.SIGKILL)
        sys.stderr.write("lcpty.py: %s timed out\n" % " ".join(sys.argv[1:]))
        break
    if not select.select([master], [], [], left)[0]:
        continue
    try:
        chunk = os.read(master, 65536)
    except OSError as e:
        if e.errno != errno.EIO:
            raise
        chunk = b""
    if not chunk:
        break
    data += chunk
status = os.waitpid(pid, 0)[1]
with open(os.environ["PTY_OUT"], "wb") as f:
    f.write(data)
if os.WIFSIGNALED(status):
    sys.exit(128 + os.WTERMSIG(status))
sys.exit(os.WEXITSTATUS(status))
EOF

# pty_run COLS [VAR=value...] -- ARGS...: like run, with lc on a terminal
# COLS columns wide; what lc prints on it goes to $TMP/out.
pty_run() {
    PTY_COLS=$1
    PTY_OUT=$TMP/out
    PTY_ERR=$TMP/err
    export PTY_COLS PTY_OUT PTY_ERR
    shift
    rm -f "$TMP/pager/"*
    lc_env "$@" </dev/null
    st=$?
    unset PTY_COLS PTY_OUT PTY_ERR
}

# A stand-in pager: records $LESS and its input in $STUB_DIR under its own
# name. With STUB_INT set it then sends SIGINT to its process group, which
# lc is in, as typing ^C in a pager does; like less, it ignores SIGINT.
stub_pager='#!/bin/sh
trap "" INT
name=${0##*/}
printf "%s\n" "${LESS-(unset)}" >"$STUB_DIR/$name.env"
while IFS= read -r line || [ -n "$line" ]; do
    printf "%s\n" "$line"
done >"$STUB_DIR/$name.in"
if [ -n "$STUB_INT" ]; then
    kill -s INT 0
fi
exit 0
'

# paged DESC NAME LESS: the last pty_run exited 0 after sending the
# coloured statement of problem 1 to the stub pager NAME, with $LESS set to
# LESS, and printed nothing itself.
paged() {
    if [ "$st" != 0 ]; then
        bad "$1" "exit status $st, expected 0"
    elif [ -s "$TMP/err" ]; then
        bad "$1" "unexpected stderr: $(head -n 3 "$TMP/err")"
    elif [ -s "$TMP/out" ]; then
        bad "$1" "printed on the terminal itself: $(head -n 3 "$TMP/out")"
    elif [ ! -f "$TMP/pager/$2.in" ]; then
        bad "$1" "the pager '$2' did not run"
    elif ! cmp -s "$TMP/pager/$2.in" "$TMP/show-color"; then
        bad "$1" "the pager '$2' got other text than 'lc show 1 --color=always'"
    elif [ "$(cat "$TMP/pager/$2.env")" != "$3" ]; then
        bad "$1" "LESS was '$(cat "$TMP/pager/$2.env")', expected '$3'"
    else
        ok
    fi
}

# direct DESC FILE [any]: the last pty_run exited 0 after printing FILE on
# the terminal itself, without a pager; stderr must be empty unless "any".
direct() {
    if [ "$st" != 0 ]; then
        bad "$1" "exit status $st, expected 0"
    elif [ "$3" != any ] && [ -s "$TMP/err" ]; then
        bad "$1" "unexpected stderr: $(head -n 3 "$TMP/err")"
    elif [ -n "$(ls "$TMP/pager")" ]; then
        bad "$1" "a pager ran: $(ls "$TMP/pager" | tr '\n' ' ')"
    elif ! cmp -s "$TMP/out" "$2"; then
        bad "$1" "the terminal shows something else"
        diff "$2" "$TMP/out" | head -n 10
    else
        ok
    fi
}

pty_note=
if [ -z "$PYTHON" ]; then
    pty_note="python3 not found"
elif ! "$PYTHON" -c 'import fcntl, pty, select, struct, termios' 2>/dev/null; then
    pty_note="python3 has no pty support"
else
    mkdir "$TMP/stubs" "$TMP/morebin" "$TMP/pager"
    run -- version
    cp "$TMP/out" "$TMP/version"
    pty_run 80 -- version
    if [ "$st" != 0 ] || ! cmp -s "$TMP/out" "$TMP/version"; then
        pty_note="cannot run lc on a pseudo-terminal"
    fi
fi
if [ -n "$pty_note" ]; then
    echo "note: skipped the terminal tests ($pty_note)"
else
    printf '%s' "$stub_pager" >"$TMP/stubs/less"
    printf '%s' "$stub_pager" >"$TMP/stubs/mypager"
    printf '%s' "$stub_pager" >"$TMP/morebin/more"
    chmod +x "$TMP/stubs/less" "$TMP/stubs/mypager" "$TMP/morebin/more"
    STUBS=$TMP/stubs:$PATH
    PG=$TMP/pager

    # What the pager or the terminal should get.
    run -- show 1 --color=always
    cp "$TMP/out" "$TMP/show-color"
    run -- list --color=always
    cp "$TMP/out" "$TMP/list-color"
    run -- list --color=never
    cp "$TMP/out" "$TMP/list-plain"
    run -- list --json
    cp "$TMP/out" "$TMP/list-json"
    run -- show 1 --json
    cp "$TMP/out" "$TMP/show-json"
    run COLUMNS=40 LC_PROBLEMS=$CLIFIX -- show 1
    cp "$TMP/out" "$TMP/sink-40"
    run COLUMNS=100 LC_PROBLEMS=$CLIFIX -- show 1
    cp "$TMP/out" "$TMP/sink-100"

    pty_run 80 PATH="$STUBS" STUB_DIR="$PG" -- show 1
    paged "lc show 1 on a terminal" less FRX
    pty_run 80 PATH="$STUBS" STUB_DIR="$PG" LESS=-S -- show 1
    paged "LESS=-S lc show 1 on a terminal" less -S
    pty_run 80 PATH="$STUBS" STUB_DIR="$PG" PAGER='less -R' -- show 1
    paged "PAGER='less -R' lc show 1 on a terminal" less FRX
    pty_run 80 PATH="$STUBS" STUB_DIR="$PG" PAGER=mypager -- show 1
    paged "PAGER=mypager lc show 1 on a terminal" mypager '(unset)'
    pty_run 80 PATH="$TMP/morebin" STUB_DIR="$PG" -- show 1
    paged "lc show 1 on a terminal, no less but more" more '(unset)'
    pty_run 80 PATH="$STUBS" STUB_DIR="$PG" STUB_INT=1 -- show 1
    paged "lc show 1 on a terminal, ^C in the pager" less FRX

    pty_run 80 PATH="$STUBS" STUB_DIR="$PG" PAGER=cat -- show 1
    direct "PAGER=cat lc show 1 on a terminal" "$TMP/show-color"
    pty_run 80 PATH="$STUBS" STUB_DIR="$PG" PAGER= -- show 1
    direct "PAGER= lc show 1 on a terminal" "$TMP/show-color"
    pty_run 80 PATH="$STUBS" STUB_DIR="$PG" PAGER=' ' -- show 1
    direct "PAGER=' ' lc show 1 on a terminal" "$TMP/show-color"
    pty_run 80 PATH="$STUBS" STUB_DIR="$PG" -- show 1 --no-pager
    direct "lc show 1 --no-pager on a terminal" "$TMP/show-color"
    pty_run 80 PATH="$STUBS" STUB_DIR="$PG" -- show 1 --json
    direct "lc show 1 --json on a terminal" "$TMP/show-json"
    pty_run 80 PATH="$TMP/empty" STUB_DIR="$PG" -- show 1
    direct "lc show 1 on a terminal, no pager installed" "$TMP/show-color"
    pty_run 80 PATH="$STUBS" STUB_DIR="$PG" PAGER=no-such-pager -- show 1
    direct "PAGER=no-such-pager lc show 1 on a terminal" "$TMP/show-color" any
    pty_run 80 PATH="$STUBS" STUB_DIR="$PG" PAGER="$CLIFIX" -- show 1
    direct "PAGER=<a directory> lc show 1 on a terminal" "$TMP/show-color" any

    pty_run 80 -- list
    direct "lc list on a terminal" "$TMP/list-color"
    pty_run 80 NO_COLOR= -- list
    direct "NO_COLOR= lc list on a terminal" "$TMP/list-color"
    pty_run 80 NO_COLOR=1 -- list
    direct "NO_COLOR=1 lc list on a terminal" "$TMP/list-plain"
    pty_run 80 TERM=dumb -- list
    direct "TERM=dumb lc list on a terminal" "$TMP/list-plain"
    pty_run 80 -- list --color=never
    direct "lc list --color=never on a terminal" "$TMP/list-plain"
    pty_run 80 -- list --json
    direct "lc list --json on a terminal" "$TMP/list-json"

    pty_run 40 COLUMNS= LC_PROBLEMS=$CLIFIX -- show 1 --no-pager --color=never
    direct "lc show 1 on a terminal 40 columns wide" "$TMP/sink-40"
    pty_run 150 COLUMNS= LC_PROBLEMS=$CLIFIX -- show 1 --no-pager --color=never
    direct "lc show 1 on a terminal 150 columns wide" "$TMP/sink-100"
    pty_run 40 COLUMNS=100 LC_PROBLEMS=$CLIFIX -- show 1 --no-pager --color=never
    direct "COLUMNS=100 lc show 1 on a terminal 40 columns wide" "$TMP/sink-100"
fi

# ------------------------------------------------------------------

if [ -z "$PYTHON" ]; then
    echo "note: python3 not found; JSON output was checked for one line only"
fi
if [ -n "$UPDATE" ]; then
    echo "e2e: $updated golden file(s) updated"
fi
echo "e2e: $pass passed, $fail failed"
[ "$fail" = 0 ]
