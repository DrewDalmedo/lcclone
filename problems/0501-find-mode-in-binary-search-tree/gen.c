/* A search tree whose values may repeat: a non-decreasing list of values is
 * laid over a shape in order, so copies land on both sides of a node.
 *
 * At full size, 10^4 nodes:
 *   1  left path, every value 10^5 (one mode, count 10^4)
 *   2  right path, all distinct, from -10^5 to 10^5 (10^4 modes)
 *   3  zigzag, runs of 2, and a run of 3 at the largest value (the last run)
 *   4  random shape, random runs of 1 to 4 (many tied modes)
 *   5  complete tree, 300 copies of -10^5 first, then short runs
 *   6  left path, half -1 and half 0 (two modes, count 5000)
 *   7  comb (a right path with a left leaf on each node), 1000 copies of 7
 *      in the middle, all else distinct */
static void comb_shape(gen_tree *t, int n)
{
    int p = tree_add(t, -1, 0, 0);

    while (t->n < n) {
        tree_add(t, p, 0, 0);
        if (t->n < n)
            p = tree_add(t, p, 1, 0);
    }
}

static void small_values(int *v, int n)
{
    int i, step;

    v[0] = rnd_chance(10) ? -100000 : (int)rnd(-3, 2);
    for (i = 1; i < n; i++) {
        step = rnd_chance(55) ? 0 : (int)rnd(1, 3);
        v[i] = v[i - 1] + step;
    }
    if (rnd_chance(10)) {
        step = 100000 - v[n - 1];
        for (i = 0; i < n; i++)
            v[i] += step;
    }
}

static void full_values(int *v, int n, int shape)
{
    int i, val, left;

    switch (shape) {
    case 0:
        for (i = 0; i < n; i++)
            v[i] = 100000;
        break;
    case 1:
        for (i = 0; i < n; i++)
            v[i] = -100000 + 20 * i;
        v[n - 1] = 100000;
        break;
    case 2:
        for (i = 0; i < n - 3; i++)
            v[i] = -90000 + i / 2;
        v[n - 3] = v[n - 2] = v[n - 1] = 100000;
        break;
    case 3:
        val = -100000;
        for (i = 0; i < n;) {
            left = (int)rnd(1, 4);
            while (left-- > 0 && i < n)
                v[i++] = val;
            val += (int)rnd(1, 3);
        }
        break;
    case 4:
        for (i = 0; i < 300; i++)
            v[i] = -100000;
        val = -99999;
        while (i < n) {
            left = (int)rnd(1, 3);
            while (left-- > 0 && i < n)
                v[i++] = val;
            val++;
        }
        break;
    case 5:
        for (i = 0; i < n; i++)
            v[i] = i < n / 2 ? -1 : 0;
        break;
    default:
        for (i = 0; i < 4500; i++)
            v[i] = i - 5000;
        for (; i < 5500; i++)
            v[i] = 7;
        for (; i < n; i++)
            v[i] = 8 + (i - 5500);
        break;
    }
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(1, 9) : 10000;
    int shape = small ? (int)rnd(0, 5) : (int)((gen_seed() - 1) % 7);
    int *v = malloc((size_t)n * sizeof *v);
    int *order = malloc((size_t)n * sizeof *order);
    int i;

    if (small) {
        if (shape == 0)
            tree_chain(t, n, TREE_LEFT);
        else if (shape == 1)
            tree_chain(t, n, TREE_RIGHT);
        else if (shape == 2)
            tree_chain(t, n, TREE_ANY);
        else
            tree_random(t, n);
        small_values(v, n);
    } else {
        if (shape == 0 || shape == 5)
            tree_chain(t, n, TREE_LEFT);
        else if (shape == 1)
            tree_chain(t, n, TREE_RIGHT);
        else if (shape == 2)
            tree_chain(t, n, TREE_ZIGZAG);
        else if (shape == 3)
            tree_random(t, n);
        else if (shape == 4)
            tree_complete(t, n);
        else
            comb_shape(t, n);
        full_values(v, n, shape);
    }
    tree_inorder(t, order);
    for (i = 0; i < n; i++)
        t->val[order[i]] = v[i];
    put_tree(t);
    tree_free(t);
    free(v);
    free(order);
}
