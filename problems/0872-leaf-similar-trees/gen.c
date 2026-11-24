/* Two trees. Small: a tiny tree and a comb-shaped one holding its fringe,
 * sometimes with one change, or another tree. At full size, 200 nodes each:
 * seed 1 is a left path against a right path with the same leaf (true), 2 a
 * zigzag against a left path with another leaf, 3 a complete tree (100 leaves)
 * against a comb with its fringe (true), 4 the same with the last leaf changed,
 * 5 with two neighbouring leaves swapped, 6 a comb of 100 leaves against one
 * holding its first 99, 7 a random shape against a comb with its fringe (true),
 * 8 a complete tree whose leaves are all 0 against a comb likewise (true), and
 * 9 is seed 6 with the two trees the other way round. Seeds 10 and 11 are a
 * comb going left against one going right, 100 leaves each, with the same fringe
 * (true), and with the last leaf changed (false). */

/* A comb with the given number of leaves under pad one-child nodes: each spine
 * node has a leaf on its left and the next spine node on its right. It has
 * pad + 2 * leaves - 1 nodes, and ids gets its leaves from left to right. */
static void make_comb(gen_tree *t, int pad, int leaves, int *ids)
{
    int cur = -1, i;

    for (i = 0; i < pad; i++)
        cur = tree_add(t, cur, i % 2, 0);
    for (i = 0; i < leaves - 1; i++) {
        cur = tree_add(t, cur, 1, 0);
        ids[i] = tree_add(t, cur, 0, 0);
    }
    ids[leaves - 1] = tree_add(t, cur, 1, 0);
}

/* Like make_comb with the sides swapped: the spine goes left, with a leaf on
 * the right of each spine node. */
static void make_left_comb(gen_tree *t, int pad, int leaves)
{
    int cur = -1, i;

    for (i = 0; i < pad; i++)
        cur = tree_add(t, cur, i % 2, 0);
    for (i = 0; i < leaves - 1; i++) {
        cur = tree_add(t, cur, 0, 0);
        tree_add(t, cur, 1, 0);
    }
    tree_add(t, cur, 0, 0);
}

/* The leaves of t from left to right; returns how many. */
static int fringe_of(const gen_tree *t, int *ids)
{
    int *order = malloc((size_t)(t->n + 1) * sizeof *order);
    int i, k = 0;

    tree_inorder(t, order);
    for (i = 0; i < t->n; i++) {
        int x = order[i];

        if (t->left[x] < 0 && t->right[x] < 0)
            ids[k++] = x;
    }
    free(order);
    return k;
}

static void fill_values(gen_tree *t, int top)
{
    int i;

    for (i = 0; i < t->n; i++)
        t->val[i] = (int)rnd(0, top);
}

static void gen_small(void)
{
    gen_tree *a = tree_new(), *b = tree_new();
    int n = (int)rnd(1, 7), kind = (int)rnd(0, 4), ia[8], ib[8], na, i;

    tree_random(a, n);
    fill_values(a, 2);
    na = fringe_of(a, ia);
    if (kind == 4) {
        int m = (int)rnd(1, 7);

        tree_random(b, m);
        fill_values(b, 2);
    } else {
        int pad = (int)rnd(0, 2), keep = kind == 2 && na > 1 ? na - 1 : na;

        make_comb(b, pad, keep, ib);
        fill_values(b, 2);
        for (i = 0; i < keep; i++)
            b->val[ib[i]] = a->val[ia[i]];
        if (kind == 1) {
            int who = (int)rnd(0, keep - 1), v = b->val[ib[who]];

            b->val[ib[who]] = v == 2 ? 1 : v + 1;
        } else if (kind == 3 && keep > 1) {
            int who = (int)rnd(0, keep - 2), v = b->val[ib[who]];

            b->val[ib[who]] = b->val[ib[who + 1]];
            b->val[ib[who + 1]] = v;
        }
    }
    put_tree(a);
    put_tree(b);
    tree_free(a);
    tree_free(b);
}

static void gen_full(int s)
{
    gen_tree *a = tree_new(), *b = tree_new();
    int n = 200, ia[128], ib[128], na, i;

    if (s < 2) {
        tree_chain(a, n, s ? TREE_ZIGZAG : TREE_LEFT);
        tree_chain(b, n, s ? TREE_LEFT : TREE_RIGHT);
        fill_values(a, 200);
        fill_values(b, 200);
        a->val[n - 1] = 200;
        b->val[n - 1] = s ? 199 : 200;
    } else if (s >= 9) {
        make_left_comb(a, 1, 100);
        make_comb(b, 1, 100, ib);
        fill_values(a, 200);
        fill_values(b, 200);
        na = fringe_of(a, ia);
        for (i = 0; i < na; i++)
            b->val[ib[i]] = a->val[ia[i]];
        if (s == 10) {
            int v = b->val[ib[na - 1]];

            b->val[ib[na - 1]] = v == 200 ? 199 : v + 1;
        }
    } else if (s == 5 || s == 8) {
        make_comb(a, 1, 100, ia);
        make_comb(b, 3, 99, ib);
        fill_values(a, 200);
        fill_values(b, 200);
        for (i = 0; i < 99; i++)
            b->val[ib[i]] = a->val[ia[i]];
    } else {
        if (s == 6)
            tree_random(a, n);
        else
            tree_complete(a, n);
        fill_values(a, 200);
        na = fringe_of(a, ia);
        make_comb(b, n - (2 * na - 1), na, ib);
        fill_values(b, 200);
        if (s == 4) {
            a->val[ia[49]] = 3;
            a->val[ia[50]] = 200;
        } else if (s == 7) {
            for (i = 0; i < n; i++) {
                a->val[i] = 200;
                b->val[i] = 200;
            }
            for (i = 0; i < na; i++)
                a->val[ia[i]] = 0;
        }
        for (i = 0; i < na; i++)
            b->val[ib[i]] = a->val[ia[i]];
        if (s == 3) {
            int v = a->val[ia[na - 1]];

            b->val[ib[na - 1]] = v == 200 ? 199 : v + 1;
        } else if (s == 4) {
            b->val[ib[49]] = 200;
            b->val[ib[50]] = 3;
        }
    }
    put_tree(s == 8 ? b : a);
    put_tree(s == 8 ? a : b);
    tree_free(a);
    tree_free(b);
}

void gen(int small)
{
    if (small)
        gen_small();
    else
        gen_full((int)((gen_seed() - 1) % 11));
}
