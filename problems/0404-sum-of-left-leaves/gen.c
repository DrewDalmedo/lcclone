/* A binary tree of values within [-1000, 1000]. At full size, 1000 nodes:
 *   seed 1  left path (one left leaf, at the bottom);
 *   seed 2  right path (no left leaf: 0);
 *   seed 3  zigzag ending on a right child (0);
 *   seed 4  right spine with a left leaf on each node, all 1000 (5 * 10^5);
 *   seed 5  the same, all -1000 (-5 * 10^5);
 *   seed 6  left spine with a right leaf on each node (no left leaf: 0);
 *   seed 7  complete tree, random values;
 *   seed 8  a random shape, random values;
 *   seed 9  right spine of 333 nodes, each with a two-node hook on its left;
 *   seed 10 perfect tree of 9 levels with 163 such hooks under its bottom row. */

/* `spine` nodes down one side, each with a leaf on the other side. */
static void comb_shape(gen_tree *t, int spine, int side)
{
    int last = -1, i;

    for (i = 0; i < spine; i++) {
        last = tree_add(t, last, side, 0);
        tree_add(t, last, !side, 0);
    }
}

/* Under node p: a left child with a left leaf of its own. */
static int left_hook(gen_tree *t, int p)
{
    int l = tree_add(t, p, 0, 0);

    return tree_add(t, l, 0, 0);
}

static void hook_spine(gen_tree *t, int spine)
{
    int last = -1, i;

    for (i = 0; i < spine; i++) {
        last = tree_add(t, last, 1, 0);
        left_hook(t, last);
    }
    tree_add(t, last, 1, 0);
}

/* A perfect tree of 511 nodes; each of the first `hooks` nodes of its bottom
 * row gets a hook and a right leaf. */
static void wide_hooks(gen_tree *t, int hooks)
{
    int i;

    tree_complete(t, 511);
    for (i = 0; i < hooks; i++) {
        left_hook(t, 255 + i);
        tree_add(t, 255 + i, 1, 0);
    }
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int i;

    if (small) {
        int n = (int)rnd(1, 8), shape = (int)rnd(0, 5);

        if (shape == 0)
            tree_chain(t, n, TREE_LEFT);
        else if (shape == 1)
            tree_chain(t, n, TREE_RIGHT);
        else if (shape == 2)
            tree_chain(t, n, TREE_ANY);
        else if (shape == 3)
            tree_complete(t, n);
        else if (shape == 4 && n >= 2)
            comb_shape(t, n / 2, (int)rnd(0, 1));
        else
            tree_random(t, n);
        n = t->n;
        for (i = 0; i < n; i++)
            t->val[i] = (int)rnd(-3, 3);
    } else {
        int n = 1000, shape = (int)((gen_seed() - 1) % 10);

        if (shape == 0)
            tree_chain(t, n, TREE_LEFT);
        else if (shape == 1)
            tree_chain(t, n, TREE_RIGHT);
        else if (shape == 2)
            tree_chain(t, n, TREE_ZIGZAG);
        else if (shape == 3 || shape == 4)
            comb_shape(t, n / 2, 1);
        else if (shape == 5)
            comb_shape(t, n / 2, 0);
        else if (shape == 6)
            tree_complete(t, n);
        else if (shape == 8)
            hook_spine(t, 333);
        else if (shape == 9)
            wide_hooks(t, 163);
        else
            tree_random(t, n);
        for (i = 0; i < n; i++)
            t->val[i] = shape == 3 ? 1000 : shape == 4 ? -1000 : (int)rnd(-1000, 1000);
    }
    put_tree(t);
    tree_free(t);
}
