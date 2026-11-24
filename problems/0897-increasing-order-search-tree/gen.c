/* A search tree of distinct values. At full size, 100 nodes from 0 to 1000:
 *   1  left path (every link turns around)
 *   2  right path (already a ladder)
 *   3  zigzag
 *   4  complete tree
 *   5  random shape
 *   6  caterpillar with its spine to the left, a right leaf on each spine node
 *   7  caterpillar with its spine to the right, a left leaf on each
 *   8  path turning at random */
static void grow_caterpillar(gen_tree *t, int n, int side)
{
    int at = tree_add(t, -1, 0, 0), next;

    while (t->n < n) {
        next = tree_add(t, at, side, 0);
        if (t->n < n)
            tree_add(t, at, !side, 0);
        at = next;
    }
}

/* n sorted distinct values of [lo, hi], each set as likely. */
static void pick_sorted(int *v, int n, int lo, int hi)
{
    int need = n, x;
    long long r;

    for (x = lo; x <= hi && need > 0; x++) {
        r = rnd(1, (long long)hi - x + 1);
        if (r <= need) {
            v[n - need] = x;
            need--;
        }
    }
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = small ? (int)rnd(0, 7) : (int)((gen_seed() - 1) % 8);
    int i;
    int *v = malloc((size_t)n * sizeof *v);
    int *order = malloc((size_t)n * sizeof *order);

    if (shape == 0)
        tree_chain(t, n, TREE_LEFT);
    else if (shape == 1)
        tree_chain(t, n, TREE_RIGHT);
    else if (shape == 2)
        tree_chain(t, n, TREE_ZIGZAG);
    else if (shape == 3)
        tree_complete(t, n);
    else if (shape == 4)
        tree_random(t, n);
    else if (shape == 5)
        grow_caterpillar(t, n, 0);
    else if (shape == 6)
        grow_caterpillar(t, n, 1);
    else
        tree_chain(t, n, TREE_ANY);

    if (small) {
        pick_sorted(v, n, 0, 12);
        if (rnd_chance(15))
            v[n - 1] = 1000;
    } else {
        for (i = 0; i < n; i++)
            v[i] = i * 10 + (int)rnd(0, 9);
        v[0] = 0;
        v[n - 1] = 1000;
    }
    tree_inorder(t, order);
    for (i = 0; i < n; i++)
        t->val[order[i]] = v[i];
    put_tree(t);
    tree_free(t);
    free(v);
    free(order);
}
