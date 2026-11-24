/* A binary tree. At full size, 10^5 nodes: seed 1 is a path of left
 * children, 2 of right children, 3 a zigzag, 4 a complete tree, 5 a
 * caterpillar (a spine of 5 * 10^4 nodes, each with one leaf), 6 a long left
 * arm beside a right arm of 10 nodes, 7 a path of 5 * 10^4 nodes above a
 * complete tree, 8 the shape with the widest level (49996 nodes), 9 a
 * random shape, and 10 a caterpillar with its spine down the right. */

/* The levels above the widest one halve, rounding up; the nodes left over
 * hang below its last node. */
static void widest_level_shape(gen_tree *t, int n)
{
    int rows[32], nrows = 0, w = (n + 1) / 2, used, i, k, start = 0, last;

    for (;; w--) {
        int x = w;

        for (used = x; x > 1; used += x)
            x = (x + 1) / 2;
        if (used <= n)
            break;
    }
    for (k = w;; k = (k + 1) / 2) {
        rows[nrows++] = k;
        if (k == 1)
            break;
    }
    tree_add(t, -1, 0, 0);
    for (i = nrows - 2; i >= 0; i--) {
        for (k = 0; k < rows[i]; k++)
            tree_add(t, start + k / 2, k % 2, 0);
        start += rows[i + 1];
    }
    last = t->n - 1;
    for (i = used; i < n; i++)
        last = tree_add(t, last, 0, 0);
}

/* A spine down one side (side 0 is left), each of its nodes with a leaf on
 * the other. An explicit stack that pushes both children holds a leaf per
 * level on one of the two sides. */
static void caterpillar_shape(gen_tree *t, int n, int side)
{
    int m = n / 2, last, i;

    last = tree_add(t, -1, 0, 0);
    for (i = 1; i < m; i++) {
        int next = tree_add(t, last, side, 0);

        tree_add(t, last, !side, 0);
        last = next;
    }
    if (n >= 2)
        tree_add(t, last, !side, 0);
    if (n % 2 && n >= 3)
        tree_add(t, last, side, 0);
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(0, 8) : 100000, i;
    int shape = small ? (int)rnd(0, 5) : (int)((gen_seed() - 1) % 10);

    if (small) {
        if (shape == 0)
            tree_chain(t, n, TREE_LEFT);
        else if (shape == 1)
            tree_chain(t, n, TREE_RIGHT);
        else if (shape == 2)
            tree_chain(t, n, TREE_ANY);
        else if (shape == 3)
            tree_complete(t, n);
        else if (shape == 4 && n > 0)
            caterpillar_shape(t, n, 0);
        else
            tree_random(t, n);
    } else if (shape == 0) {
        tree_chain(t, n, TREE_LEFT);
    } else if (shape == 1) {
        tree_chain(t, n, TREE_RIGHT);
    } else if (shape == 2) {
        tree_chain(t, n, TREE_ZIGZAG);
    } else if (shape == 3) {
        tree_complete(t, n);
    } else if (shape == 4 || shape == 9) {
        caterpillar_shape(t, n, shape == 9);
    } else if (shape == 5) {
        int l, r = 0;

        tree_add(t, -1, 0, 0);
        l = tree_add(t, 0, 0, 0);
        for (i = 0; i < 10; i++)
            r = tree_add(t, r, 1, 0);
        for (i = 12; i < n; i++)
            l = tree_add(t, l, 0, 0);
    } else if (shape == 6) {
        int m = n / 2, last = -1, base;

        for (i = 0; i < m; i++)
            last = tree_add(t, last, 0, 0);
        base = t->n;
        for (i = 0; i < n - m; i++)
            tree_add(t, i ? base + (i - 1) / 2 : last, i ? (i - 1) % 2 : 0, 0);
    } else if (shape == 7) {
        widest_level_shape(t, n);
    } else {
        tree_random(t, n);
    }
    for (i = 0; i < n; i++)
        t->val[i] = (int)(small ? rnd(-3, 3) : rnd(-1000, 1000));
    put_tree(t);
    tree_free(t);
}
