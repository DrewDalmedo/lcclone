/* A binary tree of values within [-100, 100]. At full size, 100 nodes:
 *   seed 1  left path, all -100 (the longest text, 598 characters);
 *   seed 2  right path, all -100;
 *   seed 3  zigzag with random values;
 *   seed 4  complete tree, all 100 (many equal texts);
 *   seed 5  a path into a caterpillar, 34 leaves, all -100 (the most text,
 *           10432 characters);
 *   seed 6  caterpillar with 50 leaves and random values;
 *   seed 7  a random shape with random values;
 *   seed 8  caterpillar with 50 leaves down the left, random values. */

/* A spine of `spine` nodes down one side; the last `legs` nodes above its
 * bottom each carry a leaf on the other side. */
static void legged_spine(gen_tree *t, int spine, int legs, int side)
{
    int last = -1, i;

    for (i = 0; i < spine; i++) {
        last = tree_add(t, last, side, 0);
        if (i >= spine - 1 - legs && i < spine - 1)
            tree_add(t, last, !side, 0);
    }
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int i;

    if (small) {
        int n = (int)rnd(1, 8), shape = (int)rnd(0, 4);

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

            t->val[i] = pick == 0 ? -100 : pick == 1 ? 100 : (int)rnd(-2, 2);
        }
    } else {
        int n = 100, shape = (int)((gen_seed() - 1) % 8);

        if (shape == 0)
            tree_chain(t, n, TREE_LEFT);
        else if (shape == 1)
            tree_chain(t, n, TREE_RIGHT);
        else if (shape == 2)
            tree_chain(t, n, TREE_ZIGZAG);
        else if (shape == 3)
            tree_complete(t, n);
        else if (shape == 4)
            legged_spine(t, 67, 33, 0);
        else if (shape == 5)
            legged_spine(t, 51, 49, 1);
        else if (shape == 7)
            legged_spine(t, 51, 49, 0);
        else
            tree_random(t, n);
        for (i = 0; i < n; i++)
            t->val[i] = shape == 3 ? 100
                        : shape <= 1 || shape == 4 ? -100
                                                   : (int)rnd(-100, 100);
    }
    put_tree(t);
    tree_free(t);
}
