/*
 * lc_gen.h: what a pack's gen.c can use without an #include
 * (docs/authoring.md, "gen.c"). lc builds gen.c as C11 with this file
 * first and lc_gen_main.c after it, into a program that prints test
 * cases.
 *
 * gen.c defines one function:
 *
 *     void gen(int small)
 *
 * which prints one case: one line per input, in the order of the
 * problem's parameters (then its hidden inputs), each a literal as in
 * tests.txt. `small` asks for a small case, quick for a slow brute-force
 * solution to work out; otherwise make a full-size one, up to the
 * problem's constraints.
 *
 * The random numbers come from rnd() and friends, seeded from the case's
 * seed, so a seed always gives the same case on every system. Don't use
 * rand() or the time: then cases would differ from machine to machine.
 *
 *     void gen(int small)
 *     {
 *         int n = small ? (int)rnd(2, 8) : 10000, i;
 *         int *nums = malloc(n * sizeof *nums);
 *
 *         for (i = 0; i < n; i++)
 *             nums[i] = (int)rnd(-1000000000, 1000000000);
 *         put_ints(nums, n);
 *         put_int(rnd(-2000000000, 2000000000));
 *         free(nums);
 *     }
 */
#ifndef LC_GEN_H
#define LC_GEN_H

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

void gen(int small);

/* ------------------------------------------------------------------ */
/* Random numbers                                                      */
/* ------------------------------------------------------------------ */

static uint64_t lc_gen_state;
static unsigned long long lc_gen_seed_value;

/* splitmix64: small, fast, and the same everywhere. */
static uint64_t rnd_u64(void)
{
    uint64_t z = (lc_gen_state += 0x9e3779b97f4a7c15ull);

    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
    return z ^ (z >> 31);
}

/* Starts the numbers for the case with this seed. */
static void lc_gen_start(unsigned long long seed)
{
    lc_gen_seed_value = seed;
    lc_gen_state = seed;
    lc_gen_state = rnd_u64(); /* so that seeds 1, 2, 3 start far apart */
}

/* The seed of the case being made. */
static unsigned long long gen_seed(void)
{
    return lc_gen_seed_value;
}

static void lc_gen_fail(const char *what)
{
    fprintf(stderr, "gen: %s\n", what);
    exit(3);
}

/* A whole number from lo to hi, both included, every one as likely. */
static long long rnd(long long lo, long long hi)
{
    uint64_t span, r, floor;

    if (hi < lo)
        lc_gen_fail("rnd(lo, hi) with lo > hi");
    span = (uint64_t)hi - (uint64_t)lo + 1;
    if (span == 0)
        return (long long)rnd_u64(); /* the whole range */
    /* Drop the few values that would make some results likelier. */
    floor = (0 - span) % span;
    do
        r = rnd_u64();
    while (r < floor);
    return (long long)((uint64_t)lo + r % span);
}

/* A number from lo up to (not including) hi. */
static double rnd_double(double lo, double hi)
{
    return lo + (hi - lo) * ((double)(rnd_u64() >> 11) / 9007199254740992.0);
}

/* 1 with a chance of `percent` in 100, else 0. */
static int rnd_chance(int percent)
{
    return rnd(1, 100) <= percent;
}

/* One of the characters of `chars` (not empty). */
static char rnd_char(const char *chars)
{
    size_t n = strlen(chars);

    if (n == 0)
        lc_gen_fail("rnd_char(\"\")");
    return chars[rnd(0, (long long)n - 1)];
}

/* Puts a[0..n) in a random order. */
static void rnd_shuffle(int *a, int n)
{
    int i;

    for (i = n - 1; i > 0; i--) {
        int k = (int)rnd(0, i), t = a[i];

        a[i] = a[k];
        a[k] = t;
    }
}

/* ------------------------------------------------------------------ */
/* Printing inputs: each put_ function prints one input line           */
/* ------------------------------------------------------------------ */

static void lc_gen_double(double v)
{
    char buf[40];
    int prec;

    if (v != v || v - v != 0)
        lc_gen_fail("put_double: a NaN or an infinity has no literal");
    /* The shortest text that reads back as v. */
    for (prec = 15; prec <= 17; prec++) {
        snprintf(buf, sizeof buf, "%.*g", prec, v);
        if (strtod(buf, NULL) == v)
            break;
    }
    fputs(buf, stdout);
}

static void lc_gen_string(const char *s, size_t n)
{
    size_t i;

    putchar('"');
    for (i = 0; i < n; i++) {
        unsigned char c = (unsigned char)s[i];

        if (c == '"' || c == '\\')
            printf("\\%c", c);
        else if (c == '\n')
            fputs("\\n", stdout);
        else if (c == '\t')
            fputs("\\t", stdout);
        else if (c < 0x20 || c == 0x7f)
            printf("\\u%04x", c);
        else
            putchar(c);
    }
    putchar('"');
}

static void put_int(long long v)
{
    printf("%lld\n", v);
}

static void put_double(double v)
{
    lc_gen_double(v);
    putchar('\n');
}

static void put_bool(int v)
{
    puts(v ? "true" : "false");
}

static void put_char(char c)
{
    lc_gen_string(&c, 1);
    putchar('\n');
}

static void put_string(const char *s)
{
    lc_gen_string(s, strlen(s));
    putchar('\n');
}

/* Any literal, as written, e.g. "[1,null,2]". */
static void put_raw(const char *literal)
{
    puts(literal);
}

static void put_ints(const int *a, int n)
{
    int i;

    putchar('[');
    for (i = 0; i < n; i++)
        printf("%s%d", i ? "," : "", a[i]);
    puts("]");
}

static void put_longs(const long long *a, int n)
{
    int i;

    putchar('[');
    for (i = 0; i < n; i++)
        printf("%s%lld", i ? "," : "", a[i]);
    puts("]");
}

static void put_doubles(const double *a, int n)
{
    int i;

    putchar('[');
    for (i = 0; i < n; i++) {
        if (i)
            putchar(',');
        lc_gen_double(a[i]);
    }
    puts("]");
}

static void put_bools(const bool *a, int n)
{
    int i;

    putchar('[');
    for (i = 0; i < n; i++)
        printf("%s%s", i ? "," : "", a[i] ? "true" : "false");
    puts("]");
}

/* A char[]: each character a one-character string. */
static void put_chars(const char *a, int n)
{
    int i;

    putchar('[');
    for (i = 0; i < n; i++) {
        if (i)
            putchar(',');
        lc_gen_string(&a[i], 1);
    }
    puts("]");
}

static void put_strings(char *const *a, int n)
{
    int i;

    putchar('[');
    for (i = 0; i < n; i++) {
        if (i)
            putchar(',');
        lc_gen_string(a[i], strlen(a[i]));
    }
    puts("]");
}

/* An int[][] of `rows` rows of `cols` ints, from a[rows * cols] by rows. */
static void put_int_grid(const int *a, int rows, int cols)
{
    int r, c;

    putchar('[');
    for (r = 0; r < rows; r++) {
        printf("%s[", r ? "," : "");
        for (c = 0; c < cols; c++)
            printf("%s%d", c ? "," : "", a[r * cols + c]);
        putchar(']');
    }
    puts("]");
}

/* A char[][], the same way. */
static void put_char_grid(const char *a, int rows, int cols)
{
    int r, c;

    putchar('[');
    for (r = 0; r < rows; r++) {
        printf("%s[", r ? "," : "");
        for (c = 0; c < cols; c++) {
            if (c)
                putchar(',');
            lc_gen_string(&a[r * cols + c], 1);
        }
        putchar(']');
    }
    puts("]");
}

/* ------------------------------------------------------------------ */
/* Lists and trees                                                     */
/* ------------------------------------------------------------------ */

/* A ListNode: its values in order, e.g. [1,2,3]. */
static void put_list(const int *a, int n)
{
    put_ints(a, n);
}

static void *lc_gen_alloc(size_t n, size_t size)
{
    void *p = calloc(n ? n : 1, size);

    if (!p)
        lc_gen_fail("out of memory");
    return p;
}

/*
 * A binary tree that gen.c builds node by node and put_tree prints. The
 * nodes are numbered from 0 in the order they are added; val, left and
 * right are indexed by that number, and a missing child is -1. Nothing
 * here recurses, so a tree may be as deep as it is long.
 */
typedef struct gen_tree {
    int n, cap;
    int root;          /* -1 while the tree is empty */
    int *val;
    int *left, *right;
} gen_tree;

static gen_tree *tree_new(void)
{
    gen_tree *t = (gen_tree *)lc_gen_alloc(1, sizeof *t);

    t->root = -1;
    return t;
}

static void tree_free(gen_tree *t)
{
    if (!t)
        return;
    free(t->val);
    free(t->left);
    free(t->right);
    free(t);
}

/* Adds a node of value v: the root when parent is -1, else the left
 * (side 0) or right (side 1) child of parent, which must have none there
 * yet. Returns the new node's number. */
static int tree_add(gen_tree *t, int parent, int side, int v)
{
    int i = t->n;

    if (parent < -1 || parent >= t->n || (parent == -1) != (t->root == -1))
        lc_gen_fail("tree_add: no such parent, or a second root");
    if (parent >= 0 && (side ? t->right : t->left)[parent] != -1)
        lc_gen_fail("tree_add: that child is there already");
    if (t->n == t->cap) {
        t->cap = t->cap ? 2 * t->cap : 64;
        t->val = (int *)realloc(t->val, (size_t)t->cap * sizeof *t->val);
        t->left = (int *)realloc(t->left, (size_t)t->cap * sizeof *t->left);
        t->right = (int *)realloc(t->right,
                                  (size_t)t->cap * sizeof *t->right);
        if (!t->val || !t->left || !t->right)
            lc_gen_fail("out of memory");
    }
    t->val[i] = v;
    t->left[i] = t->right[i] = -1;
    t->n++;
    if (parent < 0)
        t->root = i;
    else if (side)
        t->right[parent] = i;
    else
        t->left[parent] = i;
    return i;
}

/* Shapes. Each adds n nodes of value 0 to an empty tree; set t->val
 * afterwards, by node number or with tree_inorder. */

/* A random shape: each node goes to one of the free child slots, each as
 * likely (as inserting random keys into a binary search tree does). Its
 * depth is about 4.3 ln n. */
static void tree_random(gen_tree *t, int n)
{
    int *slot = (int *)lc_gen_alloc(2 * (size_t)n + 2, sizeof *slot);
    int nslots = 0, i;

    if (n <= 0)
        goto done;
    tree_add(t, -1, 0, 0);
    slot[nslots++] = 0;
    slot[nslots++] = 1;
    for (i = 1; i < n; i++) {
        int k = (int)rnd(0, nslots - 1), s = slot[k];

        slot[k] = slot[--nslots];
        tree_add(t, s / 2, s % 2, 0);
        slot[nslots++] = 2 * i;
        slot[nslots++] = 2 * i + 1;
    }
done:
    free(slot);
}

enum { TREE_LEFT, TREE_RIGHT, TREE_ZIGZAG, TREE_ANY };

/* A path of n nodes, as deep as it is long: each node the left child of
 * the one before (TREE_LEFT), the right child (TREE_RIGHT), each in turn
 * (TREE_ZIGZAG), or either at random (TREE_ANY). The deepest shape a
 * recursive solution meets. */
static void tree_chain(gen_tree *t, int n, int how)
{
    int i, last = -1;

    for (i = 0; i < n; i++) {
        int side = how == TREE_RIGHT    ? 1
                   : how == TREE_ZIGZAG ? i % 2
                   : how == TREE_ANY    ? (int)rnd(0, 1)
                                        : 0;

        last = tree_add(t, last, side, 0);
    }
}

/* A complete tree: level by level, left to right, as shallow as n nodes
 * allow. */
static void tree_complete(gen_tree *t, int n)
{
    int i;

    for (i = 0; i < n; i++)
        tree_add(t, i ? (i - 1) / 2 : -1, i ? (i - 1) % 2 : 0, 0);
}

/* Inserts v into a binary search tree (smaller to the left, the rest to
 * the right) and returns its node. Keys inserted in sorted order make a
 * chain. */
static int tree_bst_insert(gen_tree *t, int v)
{
    int p = t->root;

    if (p < 0)
        return tree_add(t, -1, 0, v);
    for (;;) {
        int side = v >= t->val[p], next = side ? t->right[p] : t->left[p];

        if (next < 0)
            return tree_add(t, p, side, v);
        p = next;
    }
}

/* The nodes in order (left subtree, node, right subtree) into
 * order[0..t->n): give a shape sorted values that way, and it is a
 * binary search tree. */
static void tree_inorder(const gen_tree *t, int *order)
{
    int *stack = (int *)lc_gen_alloc((size_t)t->n + 1, sizeof *stack);
    int top = 0, k = 0, p = t->root;

    while (p >= 0 || top > 0) {
        while (p >= 0) {
            stack[top++] = p;
            p = t->left[p];
        }
        p = stack[--top];
        order[k++] = p;
        p = t->right[p];
    }
    free(stack);
}

/* The number of levels: 0 for an empty tree. */
static int tree_depth(const gen_tree *t)
{
    int *depth = (int *)lc_gen_alloc((size_t)t->n + 1, sizeof *depth);
    int *queue = (int *)lc_gen_alloc((size_t)t->n + 1, sizeof *queue);
    int head = 0, tail = 0, most = 0;

    if (t->root >= 0) {
        queue[tail++] = t->root;
        depth[t->root] = 1;
    }
    while (head < tail) {
        int p = queue[head++], k;

        if (depth[p] > most)
            most = depth[p];
        for (k = 0; k < 2; k++) {
            int c = k ? t->right[p] : t->left[p];

            if (c >= 0) {
                depth[c] = depth[p] + 1;
                queue[tail++] = c;
            }
        }
    }
    free(depth);
    free(queue);
    return most;
}

/* A TreeNode in LeetCode's level order: [1,null,2], [] when empty. */
static void put_tree(const gen_tree *t)
{
    int *queue = (int *)lc_gen_alloc(2 * (size_t)t->n + 1, sizeof *queue);
    int head = 0, tail = 0, end, i;

    /* Slots in level order, -1 for a missing child; the nulls after the
     * last node are left out. */
    if (t->root >= 0)
        queue[tail++] = t->root;
    while (head < tail) {
        int p = queue[head++];

        if (p < 0)
            continue;
        queue[tail++] = t->left[p];
        queue[tail++] = t->right[p];
    }
    for (end = tail; end > 0 && queue[end - 1] < 0; end--)
        ;
    putchar('[');
    for (i = 0; i < end; i++) {
        if (i)
            putchar(',');
        if (queue[i] < 0)
            fputs("null", stdout);
        else
            printf("%d", t->val[queue[i]]);
    }
    puts("]");
    free(queue);
}

/*
 * An NaryNode in LeetCode's level order ([1,null,3,2,4,null,5,6]), from
 * node values and parents: node 0 is the root (parent[0] is -1), and each
 * node's children are the nodes whose parent it is, in increasing order.
 * Any parent works, so a chain (parent[i] = i - 1) gives the deepest
 * shape.
 */
static void put_nary(const int *val, const int *parent, int n)
{
    int *first = (int *)lc_gen_alloc((size_t)n + 1, sizeof *first);
    int *kids = (int *)lc_gen_alloc((size_t)n + 1, sizeof *kids);
    int *queue = (int *)lc_gen_alloc((size_t)n + 1, sizeof *queue);
    int *fill = (int *)lc_gen_alloc((size_t)n + 1, sizeof *fill);
    int i, head = 0, tail = 0, nulls = 0;

    /* The children of node p are kids[first[p] .. first[p + 1]). */
    for (i = 1; i < n; i++) {
        if (parent[i] < 0 || parent[i] >= n || parent[i] == i)
            lc_gen_fail("put_nary: a node's parent is not a node");
        first[parent[i] + 1]++;
    }
    for (i = 0; i < n; i++)
        first[i + 1] += first[i];
    for (i = 1; i < n; i++)
        kids[first[parent[i]] + fill[parent[i]]++] = i;
    putchar('[');
    if (n > 0) {
        if (parent[0] != -1)
            lc_gen_fail("put_nary: node 0 is the root, whose parent is -1");
        printf("%d", val[0]);
        queue[tail++] = 0;
        nulls = 1;
    }
    while (head < tail) {
        int p = queue[head++], k;

        for (k = first[p]; k < first[p + 1]; k++) {
            /* The nulls before a value end the groups before it; the ones
             * after the last value are left out. */
            for (; nulls > 0; nulls--)
                fputs(",null", stdout);
            printf(",%d", val[kids[k]]);
            queue[tail++] = kids[k];
        }
        nulls++;
    }
    if (tail != n)
        lc_gen_fail("put_nary: some nodes don't reach the root (a cycle)");
    puts("]");
    free(first);
    free(kids);
    free(queue);
    free(fill);
}

#endif /* LC_GEN_H */
