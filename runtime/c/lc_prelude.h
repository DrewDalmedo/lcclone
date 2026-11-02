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

/*
 * macOS's headers show more than glibc's do with _DEFAULT_SOURCE: BSD and
 * Darwin functions such as heapsort, mergesort, radixsort, a BSD qsort_r,
 * fls, strnstr and isnumber, and macros such as <math.h>'s OVERFLOW and
 * <limits.h>'s QUAD_MAX. LeetCode's C has none of them, so a solution may
 * use those names for its own helpers, and that must build here too. So
 * on macOS each such function or type is a macro for an lc_darwin_ name
 * while the headers below are read; after them, those macros and the
 * extra ones are #undef'd. (_POSIX_C_SOURCE would hide all of these, but
 * also strsep, strcasecmp, bzero, strlcpy, alloca and arc4random, which
 * glibc shows.) Names from the POSIX headers that macOS's <stdlib.h>
 * brings in, such as wait, signal and getrusage, are left alone: a
 * solution may include those headers itself, and they would stay hidden.
 */
#ifdef __APPLE__
/* <stdlib.h> */
#define arc4random_addrandom lc_darwin_arc4random_addrandom
#define arc4random_stir lc_darwin_arc4random_stir
#define atexit_b lc_darwin_atexit_b
#define bsearch_b lc_darwin_bsearch_b
#define cgetcap lc_darwin_cgetcap
#define cgetclose lc_darwin_cgetclose
#define cgetent lc_darwin_cgetent
#define cgetfirst lc_darwin_cgetfirst
#define cgetmatch lc_darwin_cgetmatch
#define cgetnext lc_darwin_cgetnext
#define cgetnum lc_darwin_cgetnum
#define cgetset lc_darwin_cgetset
#define cgetstr lc_darwin_cgetstr
#define cgetustr lc_darwin_cgetustr
#define daemon lc_darwin_daemon
#define devname lc_darwin_devname
#define devname_r lc_darwin_devname_r
#define getbsize lc_darwin_getbsize
#define getprogname lc_darwin_getprogname
#define grantpt lc_darwin_grantpt
#define heapsort lc_darwin_heapsort
#define heapsort_b lc_darwin_heapsort_b
#define mergesort lc_darwin_mergesort
#define mergesort_b lc_darwin_mergesort_b
#define posix_openpt lc_darwin_posix_openpt
#define psort lc_darwin_psort
#define psort_b lc_darwin_psort_b
#define psort_r lc_darwin_psort_r
#define ptsname lc_darwin_ptsname
#define ptsname_r lc_darwin_ptsname_r
#define qsort_b lc_darwin_qsort_b
#define qsort_r lc_darwin_qsort_r
#define radixsort lc_darwin_radixsort
#define reallocf lc_darwin_reallocf
#define setkey lc_darwin_setkey
#define setprogname lc_darwin_setprogname
#define sradixsort lc_darwin_sradixsort
#define sranddev lc_darwin_sranddev
#define srandomdev lc_darwin_srandomdev
#define strtonum lc_darwin_strtonum
#define suboptarg lc_darwin_suboptarg
#define unlockpt lc_darwin_unlockpt
/* <stdio.h> */
#define ctermid_r lc_darwin_ctermid_r
#define fgetln lc_darwin_fgetln
#define fmtcheck lc_darwin_fmtcheck
#define fpurge lc_darwin_fpurge
#define funopen lc_darwin_funopen
#define gets lc_darwin_gets
#define sys_errlist lc_darwin_sys_errlist
#define sys_nerr lc_darwin_sys_nerr
/* <string.h> and the <strings.h> it includes */
#define fls lc_darwin_fls
#define flsl lc_darwin_flsl
#define flsll lc_darwin_flsll
#define memset_pattern4 lc_darwin_memset_pattern4
#define memset_pattern8 lc_darwin_memset_pattern8
#define memset_pattern16 lc_darwin_memset_pattern16
#define strmode lc_darwin_strmode
#define strnstr lc_darwin_strnstr
#define strsignal_r lc_darwin_strsignal_r
#define swab lc_darwin_swab
#define timingsafe_bcmp lc_darwin_timingsafe_bcmp
/* <ctype.h> */
#define digittoint lc_darwin_digittoint
#define ishexnumber lc_darwin_ishexnumber
#define isideogram lc_darwin_isideogram
#define isnumber lc_darwin_isnumber
#define isphonogram lc_darwin_isphonogram
#define isrune lc_darwin_isrune
#define isspecial lc_darwin_isspecial
/* <math.h>: struct exception */
#define exception lc_darwin_exception
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

#ifdef __APPLE__
/* The names renamed above */
#undef arc4random_addrandom
#undef arc4random_stir
#undef atexit_b
#undef bsearch_b
#undef cgetcap
#undef cgetclose
#undef cgetent
#undef cgetfirst
#undef cgetmatch
#undef cgetnext
#undef cgetnum
#undef cgetset
#undef cgetstr
#undef cgetustr
#undef daemon
#undef devname
#undef devname_r
#undef getbsize
#undef getprogname
#undef grantpt
#undef heapsort
#undef heapsort_b
#undef mergesort
#undef mergesort_b
#undef posix_openpt
#undef psort
#undef psort_b
#undef psort_r
#undef ptsname
#undef ptsname_r
#undef qsort_b
#undef qsort_r
#undef radixsort
#undef reallocf
#undef setkey
#undef setprogname
#undef sradixsort
#undef sranddev
#undef srandomdev
#undef strtonum
#undef suboptarg
#undef unlockpt
#undef ctermid_r
#undef fgetln
#undef fmtcheck
#undef fpurge
#undef funopen
#undef gets
#undef sys_errlist
#undef sys_nerr
#undef fls
#undef flsl
#undef flsll
#undef memset_pattern4
#undef memset_pattern8
#undef memset_pattern16
#undef strmode
#undef strnstr
#undef strsignal_r
#undef swab
#undef timingsafe_bcmp
#undef digittoint
#undef ishexnumber
#undef isideogram
#undef isnumber
#undef isphonogram
#undef isrune
#undef isspecial
#undef exception
/* <stdio.h>'s macros for funopen */
#undef fropen
#undef fwopen
/* <math.h>'s SVID macros */
#undef DOMAIN
#undef SING
#undef OVERFLOW
#undef UNDERFLOW
#undef TLOSS
#undef PLOSS
#undef X_TLOSS
#undef HUGE
/* <limits.h>'s BSD macros */
#undef CHILD_MAX
#undef CLK_TCK
#undef GID_MAX
#undef OFF_MAX
#undef OFF_MIN
#undef OPEN_MAX
#undef PASS_MAX
#undef QUAD_MAX
#undef QUAD_MIN
#undef SIZE_T_MAX
#undef UID_MAX
#undef UQUAD_MAX
#endif

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
