/* A search tree of distinct values and a total to reach. At full size, 10^4
 * nodes:
 *   1  left path, all even values, k odd (false after trying everything)
 *   2  right path, 1 to 10^4, k = 19999: only the two deepest nodes reach it
 *   3  zigzag holding 10^4, k = 20000: only a node used twice would reach it
 *   4  random shape, random values, k the sum of two random nodes (true)
 *   5  complete tree of odd values, k = 10^5 (false, far out of reach)
 *   6  path turning at random, -10^4 to -1, k = -19999: the two smallest
 *   7  left path, 0 to 9999, k = 0: only 0 used twice would reach it
 *   8  random shape, random values, k = -10^5 (false) */
static void draw_distinct(int *v, int n, int lo, int hi)
{
    int i = 0, j, x, dup;

    while (i < n) {
        x = (int)rnd(lo, hi);
        dup = 0;
        for (j = 0; j < i; j++)
            if (v[j] == x)
                dup = 1;
        if (dup)
            continue;
        for (j = i; j > 0 && v[j - 1] > x; j--)
            v[j] = v[j - 1];
        v[j] = x;
        i++;
    }
}

/* n sorted distinct values of [lo, hi], each set as likely. */
static void sample_range(int *v, int n, int lo, int hi)
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

static int small_k(const int *v, int n)
{
    int kind = (int)rnd(0, 5), a, b;

    if (kind <= 1 && n >= 2) {
        a = (int)rnd(0, n - 2);
        b = (int)rnd(a + 1, n - 1);
        return v[a] + v[b];
    }
    if (kind <= 3) {
        a = (int)rnd(0, n - 1);
        return 2 * v[a];
    }
    if (kind == 4)
        return (int)rnd(-14, 14);
    return rnd_chance(50) ? 100000 : -100000;
}

static int full_values(int *v, int n, int shape)
{
    int i, a, b;

    switch (shape) {
    case 0:
        for (i = 0; i < n; i++)
            v[i] = -10000 + 2 * i;
        return 1;
    case 1:
        for (i = 0; i < n; i++)
            v[i] = i + 1;
        return 19999;
    case 2:
        for (i = 0; i < n; i++)
            v[i] = 1 + i;
        return 20000;
    case 3:
        sample_range(v, n, -10000, 10000);
        a = (int)rnd(0, n - 2);
        b = (int)rnd(a + 1, n - 1);
        return v[a] + v[b];
    case 4:
        for (i = 0; i < n; i++)
            v[i] = -9999 + 2 * i;
        return 100000;
    case 5:
        for (i = 0; i < n; i++)
            v[i] = -10000 + i;
        return -19999;
    case 6:
        for (i = 0; i < n; i++)
            v[i] = i;
        return 0;
    default:
        sample_range(v, n, -10000, 10000);
        return -100000;
    }
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(1, 8) : 10000;
    int shape = small ? (int)rnd(0, 5) : (int)((gen_seed() - 1) % 8);
    int *v = malloc((size_t)n * sizeof *v);
    int *order = malloc((size_t)n * sizeof *order);
    int i, k;

    if (small) {
        if (shape == 0)
            tree_chain(t, n, TREE_LEFT);
        else if (shape == 1)
            tree_chain(t, n, TREE_RIGHT);
        else if (shape == 2)
            tree_chain(t, n, TREE_ANY);
        else
            tree_random(t, n);
        if (rnd_chance(15))
            draw_distinct(v, n, -10000, 10000);
        else
            draw_distinct(v, n, -7, 7);
        k = small_k(v, n);
    } else {
        if (shape == 0 || shape == 6)
            tree_chain(t, n, TREE_LEFT);
        else if (shape == 1)
            tree_chain(t, n, TREE_RIGHT);
        else if (shape == 2)
            tree_chain(t, n, TREE_ZIGZAG);
        else if (shape == 3 || shape == 7)
            tree_random(t, n);
        else if (shape == 4)
            tree_complete(t, n);
        else
            tree_chain(t, n, TREE_ANY);
        k = full_values(v, n, shape);
    }
    tree_inorder(t, order);
    for (i = 0; i < n; i++)
        t->val[order[i]] = v[i];
    put_tree(t);
    put_int(k);
    tree_free(t);
    free(v);
    free(order);
}
