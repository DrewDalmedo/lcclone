/*
 * lc_prelude.h: what a C solution can use without an #include, as on
 * LeetCode: the usual headers and uthash. lc puts it at the top of every
 * C build, and the .clangd file it writes in the workspace makes clangd
 * read it too.
 *
 * The node structs are defined only for a problem that uses them, because
 * solutions often define a `struct Node` of their own. lc defines
 * LC_LISTNODE, LC_TREENODE or LC_NARYNODE before this file when the
 * problem's signature needs one.
 */
#ifndef LC_PRELUDE_H
#define LC_PRELUDE_H

/* strdup, M_PI and the other POSIX and BSD names glibc and musl hide in
 * strict C11 mode; other systems show them anyway. */
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef LC_LISTNODE
struct ListNode {
    int val;
    struct ListNode *next;
};
#endif

#ifdef LC_TREENODE
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};
#endif

#ifdef LC_NARYNODE
struct Node {
    int val;
    int numChildren;
    struct Node **children;
};
#endif

/* uthash (uthash.h, from https://troydhanson.github.io/uthash/), the hash
 * table macros LeetCode's C environment has. lc keeps a copy next to the
 * file it compiles, and one next to the copy of this file that .clangd
 * points at. */
#include "uthash.h"

#endif /* LC_PRELUDE_H */
