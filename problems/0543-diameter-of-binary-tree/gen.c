/* A non-empty binary tree. At full size, 10^4 nodes: seed 1 is a path of
 * left children, 2 of right children, 3 a zigzag, 4 a complete tree, 5 a
 * random shape, 6 a path of 1000 nodes ending in two arms of 4500 (the best
 * route turns far below the root and misses it), 7 a root with one leaf
 * beside a bushy subtree, and 8 and 9 caterpillars (a spine of 5000 nodes,
 * each with one leaf) with the spine down the left and the right. */
void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(1, 8) : 10000, i;
    int shape = small ? (int)rnd(0, 4) : (int)((gen_seed() - 1) % 9);

    if (shape == 0) {
        tree_chain(t, n, TREE_LEFT);
    } else if (shape == 1) {
        tree_chain(t, n, TREE_RIGHT);
    } else if (shape == 2) {
        tree_chain(t, n, small ? TREE_ANY : TREE_ZIGZAG);
    } else if (shape == 3) {
        tree_complete(t, n);
    } else if (shape == 4) {
        tree_random(t, n);
    } else if (shape == 5) {
        int last = -1, l, r;

        for (i = 0; i < 1000; i++)
            last = tree_add(t, last, 0, 0);
        l = r = last;
        for (i = 0; i < 4500; i++) {
            l = tree_add(t, l, 0, 0);
            r = tree_add(t, r, 1, 0);
        }
    } else if (shape == 6) {
        tree_add(t, -1, 0, 0);
        tree_add(t, 0, 1, 0);
        for (i = 0; i < n - 2; i++)
            tree_add(t, i ? 2 + (i - 1) / 2 : 0, i ? (i - 1) % 2 : 0, 0);
    } else {
        int last = tree_add(t, -1, 0, 0), side = shape - 7;

        for (i = 1; i < n / 2; i++) {
            int next = tree_add(t, last, side, 0);

            tree_add(t, last, !side, 0);
            last = next;
        }
        tree_add(t, last, !side, 0);
    }
    for (i = 0; i < n; i++)
        t->val[i] = (int)(small ? rnd(-3, 3) : rnd(-100, 100));
    put_tree(t);
    tree_free(t);
}
