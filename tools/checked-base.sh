#!/bin/sh
# tools/checked-base.sh: the commit a push to main compares with, for
# tools/full-packs.sh (docs/PLAN.md section 6.4).
#
#   printf '%s\n' COMMIT... | sh tools/checked-base.sh    run in a git checkout
#
# Reads the commits whose CI runs passed, newest first, and prints the
# first one in HEAD's history. A commit outside it, as after a force-push
# or when an older run is run again, is passed over. Prints nothing when
# no commit qualifies, and full-packs.sh then picks every pack.
#
# POSIX sh only.

while IFS= read -r commit; do
    [ -n "$commit" ] || continue
    if git merge-base --is-ancestor "$commit" HEAD 2>/dev/null; then
        echo "$commit"
        exit 0
    fi
done
