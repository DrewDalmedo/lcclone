/* Small cases: 2 to 12 candies whose kinds come from a pool of 1 to 6
 * labels, so that the number of kinds falls below, at and above n / 2.
 * The labels are from -3 to 3 (0 and -1 among them), and now and then one
 * is -10^5 or 10^5. Full-size ones have 10^4 candies, shuffled, with a
 * number of kinds the seed picks: all different (the slowest for comparing
 * each candy with the ones before it), a single kind, exactly n / 2, one
 * fewer or one more than n / 2, up to 300, or labels drawn at random over
 * the whole range. */
static int label(void)
{
    int v = (int)rnd(-3, 3);

    if (rnd_chance(15))
        v = rnd_chance(50) ? -100000 : 100000;
    return v;
}

void gen(int small)
{
    int pool[6];
    int n, k, i, v, shape;
    int *types;
    unsigned char *used;

    if (small) {
        n = 2 * (int)rnd(1, 6);
        k = (int)rnd(1, 6);
        types = malloc((size_t)n * sizeof *types);
        for (i = 0; i < k; i++)
            pool[i] = label();
        for (i = 0; i < n; i++)
            types[i] = pool[rnd(0, k - 1)];
        put_ints(types, n);
        free(types);
        return;
    }
    n = 10000;
    types = malloc((size_t)n * sizeof *types);
    shape = (int)(gen_seed() % 6);
    if (shape == 5) {
        for (i = 0; i < n; i++)
            types[i] = (int)rnd(-100000, 100000);
        put_ints(types, n);
        free(types);
        return;
    }
    switch (shape) {
    case 0:
        k = n;
        break;
    case 1:
        k = 1;
        break;
    case 2:
        k = n / 2;
        break;
    case 3:
        k = rnd_chance(50) ? n / 2 - 1 : n / 2 + 1;
        break;
    default:
        k = (int)rnd(2, 300);
        break;
    }
    /* k different labels first, then copies of them, then shuffle. */
    used = calloc(200001, 1);
    for (i = 0; i < k; i++) {
        do
            v = (int)rnd(-100000, 100000);
        while (used[v + 100000]);
        used[v + 100000] = 1;
        types[i] = v;
    }
    for (i = k; i < n; i++)
        types[i] = types[rnd(0, k - 1)];
    rnd_shuffle(types, n);
    put_ints(types, n);
    free(used);
    free(types);
}
