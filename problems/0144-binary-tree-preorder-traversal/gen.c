/* A binary tree whose values may repeat. At full size, 100 nodes: seed 1 is
 * a path of left children, 2 of right children, 3 a zigzag, 4 a random
 * shape, 5 a complete tree, 6 a caterpillar (a random spine with a leaf
 * beside each step), 7 a random shape with every value equal, and 8 and 9
 * combs, a spine going left or right with a leaf on the other side of each
 * step: the shapes that fill an explicit stack. */
static void caterpillar(gen_tree *t, int n)
{
    int spine = -1, added = 0;

    while (added < n) {
        int side = (int)rnd(0, 1);
        int next = tree_add(t, spine, side, 0);

        added++;
        if (spine >= 0 && added < n) {
            tree_add(t, spine, !side, 0);
            added++;
        }
        spine = next;
    }
}

static void comb(gen_tree *t, int n, int side)
{
    int spine = tree_add(t, -1, 0, 0), added = 1;

    while (added < n) {
        int next = tree_add(t, spine, side, 0);

        added++;
        if (added < n) {
            tree_add(t, spine, !side, 0);
            added++;
        }
        spine = next;
    }
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int shape = small ? (int)rnd(0, 6) : (int)((gen_seed() - 1) % 9);
    int n = small ? (int)rnd(0, 8) : 100;
    int top = small ? 3 : 100;
    int same = (int)rnd(-top, top);
    int i;

    if (shape == 0)
        tree_chain(t, n, TREE_LEFT);
    else if (shape == 1)
        tree_chain(t, n, TREE_RIGHT);
    else if (shape == 2)
        tree_chain(t, n, TREE_ZIGZAG);
    else if (shape == 4)
        tree_complete(t, n);
    else if (shape == 5)
        caterpillar(t, n);
    else if (shape == 7)
        comb(t, n, 0);
    else if (shape == 8)
        comb(t, n, 1);
    else
        tree_random(t, n);
    for (i = 0; i < n; i++)
        t->val[i] = shape == 6 ? same : (int)rnd(-top, top);
    if (!small && shape != 6) {
        t->val[0] = -100;
        t->val[n - 1] = 100;
    }
    put_tree(t);
    tree_free(t);
}
