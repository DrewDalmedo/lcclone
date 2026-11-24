/* A non-empty binary tree over the full int range. At full size, 10^4
 * nodes: seed 1 is a path of left children with random values, 2 a path of
 * right children alternating the two int limits, 3 a zigzag with random
 * values, 4 a complete tree of 2^31 - 1. Seeds 5 to 8 have the widest level
 * 10^4 nodes allow (4996 nodes): all 2^31 - 1, all -2^31, the two limits in
 * turn, and values within 1000 of either limit in turn. Seed 9 is a random
 * shape with random values, and 10 and 11 are caterpillars (a spine of 5000
 * nodes, each with a leaf) down the left and the right. */

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
    int n = small ? (int)rnd(1, 7) : 10000, i;
    int shape = small ? (int)rnd(0, 4) : (int)((gen_seed() - 1) % 11);
    const long long top = 2147483647LL, bottom = -2147483647LL - 1;

    if (small) {
        if (shape == 0)
            tree_chain(t, n, TREE_LEFT);
        else if (shape == 1)
            tree_chain(t, n, TREE_RIGHT);
        else if (shape == 2)
            tree_chain(t, n, TREE_ANY);
        else if (shape == 3)
            tree_complete(t, n);
        else
            tree_random(t, n);
        for (i = 0; i < n; i++) {
            int pick = (int)rnd(0, 5);

            t->val[i] = pick == 0   ? (int)top
                        : pick == 1 ? (int)bottom
                                    : (int)rnd(-5, 5);
        }
    } else if (shape == 0 || shape == 2 || shape == 8) {
        if (shape == 0)
            tree_chain(t, n, TREE_LEFT);
        else if (shape == 2)
            tree_chain(t, n, TREE_ZIGZAG);
        else
            tree_random(t, n);
        for (i = 0; i < n; i++)
            t->val[i] = (int)rnd(bottom, top);
    } else if (shape == 1) {
        tree_chain(t, n, TREE_RIGHT);
        for (i = 0; i < n; i++)
            t->val[i] = (int)(i % 2 ? bottom : top);
    } else if (shape == 3) {
        tree_complete(t, n);
        for (i = 0; i < n; i++)
            t->val[i] = (int)top;
    } else if (shape >= 9) {
        caterpillar_shape(t, n, shape - 9);
        for (i = 0; i < n; i++)
            t->val[i] = (int)rnd(bottom, top);
    } else {
        widest_level_shape(t, n);
        for (i = 0; i < n; i++) {
            if (shape == 4)
                t->val[i] = (int)top;
            else if (shape == 5)
                t->val[i] = (int)bottom;
            else if (shape == 6)
                t->val[i] = (int)(i % 2 ? bottom : top);
            else if (i % 2)
                t->val[i] = (int)rnd(bottom, bottom + 1000);
            else
                t->val[i] = (int)rnd(top - 1000, top);
        }
    }
    put_tree(t);
    tree_free(t);
}
