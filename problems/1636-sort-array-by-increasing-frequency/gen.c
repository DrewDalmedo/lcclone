/* Small cases have 1 to 8 values: mostly from -4..4, so that counts tie
 * often; some from a few values near the bounds (-100, -99, -1, 0, 1, 99,
 * 100); some all equal. Full-size ones have 100 values, with the seed
 * picking the shape: random over the whole range, both bounds included;
 * random from -6..6 (large counts that tie); 100 distinct values, both
 * bounds included (every count is 1); or a few distinct values, each slot
 * taking one of them. */
static const int edge[] = {-100, -99, -1, 0, 1, 99, 100};

/* Puts k distinct values from -100..100 in d[0..k), after the first
 * `have` that are already there. */
static void distinct(int *d, int have, int k)
{
    char seen[201] = {0};
    int i, v;

    for (i = 0; i < have; i++)
        seen[d[i] + 100] = 1;
    for (i = have; i < k; i++) {
        do
            v = (int)rnd(-100, 100);
        while (seen[v + 100]);
        seen[v + 100] = 1;
        d[i] = v;
    }
}

void gen(int small)
{
    int a[100], d[100];
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = small ? (int)rnd(0, 9) : (int)rnd(0, 3);
    int i, k, v;

    if (small && shape <= 5) {
        for (i = 0; i < n; i++)
            a[i] = (int)rnd(-4, 4);
    } else if (small && shape <= 8) {
        for (i = 0; i < n; i++) {
            k = (int)rnd(0, 6);
            a[i] = edge[k];
        }
    } else if (small) {
        v = (int)rnd(-100, 100);
        for (i = 0; i < n; i++)
            a[i] = v;
    } else if (shape == 0) {
        a[0] = -100;
        a[1] = 100;
        for (i = 2; i < n; i++)
            a[i] = (int)rnd(-100, 100);
    } else if (shape == 1) {
        for (i = 0; i < n; i++)
            a[i] = (int)rnd(-6, 6);
    } else if (shape == 2) {
        a[0] = -100;
        a[1] = 100;
        distinct(a, 2, n);
    } else {
        k = (int)rnd(2, 12);
        distinct(d, 0, k);
        for (i = 0; i < n; i++) {
            v = (int)rnd(0, k - 1);
            a[i] = d[v];
        }
    }
    rnd_shuffle(a, n);
    put_ints(a, n);
}
