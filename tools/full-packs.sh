#!/bin/sh
# tools/full-packs.sh: the packs CI checks in full, with lc check's slow
# checks on the full-size cases (docs/PLAN.md section 6.4).
#
#   sh tools/full-packs.sh BASE    run in a git checkout
#
# Prints "all" for every pack, or else the pack directories
# (problems/NNNN-slug) that differ between commit BASE and HEAD, one per
# line, and nothing when none does. It is every pack when there is no
# BASE, when git can't compare it with HEAD, or when lc's runtime differs
# (runtime/, lc_gen.h included): that is built into every program lc check
# runs. Packs that are gone from HEAD are left out.
#
# POSIX sh only.

base=${1-}
if [ -z "$base" ]; then
    echo all
    exit 0
fi
top=$(git rev-parse --show-toplevel) && cd "$top" || exit 2
files=$(git diff --no-color --no-renames --name-only "$base" HEAD --) || {
    echo "full-packs.sh: cannot compare $base with HEAD; checking every pack" >&2
    echo all
    exit 0
}
case "
$files" in
*"
runtime/"*)
    echo all
    exit 0
    ;;
esac
printf '%s\n' "$files" |
    sed -n 's|^\(problems/[0-9][0-9]*-[^/]*\)/.*|\1|p' |
    sort -u |
    while read -r pack; do
        if [ -d "$pack" ]; then
            echo "$pack"
        fi
    done
