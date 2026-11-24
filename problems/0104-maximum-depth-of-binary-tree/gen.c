/* A binary tree. At full size, 10^4 nodes: seed 1 is a path of left
 * children, 2 of right children, 3 a zigzag, 4 a random shape, 5 a complete
 * tree, 6 a bushy left subtree beside a long bare path on the right, 7 the
 * shape with the widest level 10^4 nodes allow (4996 nodes), and 8 and 9
 * caterpillars with a spine of 5000 nodes down the left and the right. */

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

/* A spine of n / 2 nodes going down one side (side 0 is left), each spine
 * node with a leaf on the other side. An explicit stack that pushes both
 * children holds a leaf per level on one of the two sides. */
static void caterpillar_shape(gen_tree *t, int n, int side)
{
    int last = tree_add(t, -1, 0, 0), i;

    for (i = 1; i < n / 2; i++) {
        int next = tree_add(t, last, side, 0);

        tree_add(t, last, !side, 0);
        last = next;
    }
    tree_add(t, last, !side, 0);
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(0, 7) : 10000, i;
    int shape = small ? (int)rnd(0, 4) : (int)((gen_seed() - 1) % 9);

    if (shape == 0) {
        tree_chain(t, n, TREE_LEFT);
    } else if (shape == 1) {
        tree_chain(t, n, TREE_RIGHT);
    } else if (shape == 2) {
        tree_chain(t, n, TREE_ZIGZAG);
    } else if (shape == 3) {
        tree_random(t, n);
    } else if (shape == 4) {
        tree_complete(t, n);
    } else if (shape == 5) {
        int m = n / 2, last = 0;

        tree_add(t, -1, 0, 0);
        for (i = 0; i < m; i++)
            tree_add(t, i ? 1 + (i - 1) / 2 : 0, i ? (i - 1) % 2 : 0, 0);
        for (i = m + 1; i < n; i++)
            last = tree_add(t, last, 1, 0);
    } else if (shape == 6) {
        widest_level_shape(t, n);
    } else {
        caterpillar_shape(t, n, shape - 7);
    }
    for (i = 0; i < n; i++)
        t->val[i] = (int)(small ? rnd(-3, 3) : rnd(-100, 100));
    put_tree(t);
    tree_free(t);
}
