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
    echo "FAIL: lc $1: $2"
}

# A string as a basic regular expression for sed's s/// command.
re_escape() {
    printf '%s\n' "$1" | sed 's/[]\/$*.^[]/\\&/g'
}
ROOT_RE=$(re_escape "$ROOT")
TMP_RE=$(re_escape "$TMP")
TMP_P_RE=$(re_escape "$TMP_P")

normalize() {
    sed -e "s/$TMP_P_RE/@TMP@/g" -e "s/$TMP_RE/@TMP@/g" \
        -e "s/$ROOT_RE/@ROOT@/g" -e "s/$ESC/<ESC>/g"
}

# lc_env [VAR=value...] -- ARGS...: runs lc with the test environment plus
# the given variables. A variable set to "" counts as unset for lc.
lc_env() {
    rotated=
    for a do
        shift
        if [ -z "$rotated" ] && [ "x$a" = x-- ]; then
            # shellcheck disable=SC2086 # the wrapper is split into words
            set -- "$@" $LC_E2E_WRAP "$LC"
            rotated=1
        else
            set -- "$@" "$a"
        fi
    done
    env -i PATH="$PATH" HOME="$TMP/home" XDG_DATA_HOME="$TMP/home/.local/share" \
        TERM=xterm LC_ALL=C COLUMNS=80 LC_PROBLEMS="$FIX" \
        ASAN_OPTIONS="${ASAN_OPTIONS-}" UBSAN_OPTIONS="${UBSAN_OPTIONS-}" \
        "$@"
}

# run [VAR=value...] -- ARGS...: stdout in $TMP/out, stderr in $TMP/err,
# exit status in $st.
run() {
    lc_env "$@" >"$TMP/out" 2>"$TMP/err" </dev/null
    st=$?
}

# The arguments after the first "--" as one string, for messages.
describe() {
    seen=
    d=
    for a do
        if [ -n "$seen" ]; then
            d="$d $a"
        elif [ "x$a" = x-- ]; then
            seen=1
        fi
    done
    printf '%s' "${d# }"
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

# check NAME [VAR=value...] -- ARGS...: runs lc and compares the result with
# tests/expected/NAME.txt.
check() {
    name=$1
    shift
    desc=$(describe "$@")
    run "$@"
    record >"$TMP/got"
    if json_wanted "$@"; then
        check_json "$desc"
    fi
    compare "$name" "$desc" golden
}

# same NAME [VAR=value...] -- ARGS...: like check, but the output must equal
# that of the test that wrote NAME.
same() {
    name=$1
    shift
    desc=$(describe "$@")
    run "$@"
    record >"$TMP/got"
    if json_wanted "$@"; then
        check_json "$desc"
    fi
    compare "$name" "$desc" same
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
        bad "list (no packs anywhere)" "exit $st, stderr: $(cat "$TMP/err")"
    fi
    run LC_PROBLEMS= -- list --json
    check_json "list --json (no packs anywhere)"
    if [ "$st" = 3 ] &&
        grep -q '"error":{"message":"no problem directory found' "$TMP/out"
    then
        ok
    else
        bad "list --json (no packs anywhere)" "exit $st, stdout: $(cat "$TMP/out")"
    fi
fi

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

pipe_head "list | head -1" -- list

# Outputs much larger than a pipe buffer, so that lc really sees EPIPE.
mkdir "$TMP/big" "$TMP/many"
mkdir "$TMP/big/0001-big"
awk 'BEGIN {
    printf "id: 1\nslug: big\ntitle: Big\ndifficulty: easy\n---\n"
    for (i = 0; i < 4000; i++)
        printf("Paragraph %d has enough words in it to make the output big.\n\n", i)
}' >"$TMP/big/0001-big/problem.md"
pipe_head "show | head -1 (large output)" LC_PROBLEMS="$TMP/big" -- show 1
awk -v dir="$TMP/many" 'BEGIN {
    for (i = 1; i <= 3000; i++)
        printf("%s/%04d-problem-%d\n", dir, i, i)
}' | xargs mkdir
awk -v dir="$TMP/many" 'BEGIN {
    for (i = 1; i <= 3000; i++) {
        f = sprintf("%s/%04d-problem-%d/problem.md", dir, i, i)
        printf("id: %d\nslug: problem-%d\ntitle: Problem number %d\n", i, i, i) > f
        printf("difficulty: easy\ntags: array, string\n---\nx\n") > f
        close(f)
    }
}'
pipe_head "list | head -1 (3000 problems)" LC_PROBLEMS="$TMP/many" -- list
pipe_head "list --json | head -1 (3000 problems)" LC_PROBLEMS="$TMP/many" -- list --json

# ------------------------------------------------------------------
# The repository's own problem packs load without warnings
# ------------------------------------------------------------------

if [ -d problems ]; then
    run LC_PROBLEMS=problems -- list
    expect_clean "list (LC_PROBLEMS=problems)"
    if [ ! -s "$TMP/out" ]; then
        bad "list (LC_PROBLEMS=problems)" "no problems listed"
    fi
    for id in $(awk 'NR > 1 { print $1 }' "$TMP/out"); do
        run LC_PROBLEMS=problems -- show "$id"
        expect_clean "show $id (LC_PROBLEMS=problems)"
        run LC_PROBLEMS=problems -- show "$id" --json
        expect_clean "show $id --json (LC_PROBLEMS=problems)"
        check_json "show $id --json (LC_PROBLEMS=problems)"
    done
    run LC_PROBLEMS=problems -- list --json
    expect_clean "list --json (LC_PROBLEMS=problems)"
    check_json "list --json (LC_PROBLEMS=problems)"
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
