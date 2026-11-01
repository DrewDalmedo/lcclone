/* Most cases pick a few distinct numbers and a tally for each, all
 * tallies different, then sometimes copy one tally onto another so that
 * two clash. Small cases have up to 14 entries: random numbers from
 * -3..3, or numbers from -4..4 or from the whole range (-1000, 0 and 1000
 * come up often) with tallies from 1..5. Full-size ones, with the seed
 * picking the shape: 44 numbers with the tallies 1 to 44 (990 entries),
 * with or without a clash; up to 10 numbers with large tallies; one
 * number 1000 times; or 1000 random entries. */
static int has(const int *a, int n, int v)
{
    int i;

    for (i = 0; i < n; i++)
        if (a[i] == v)
            return 1;
    return 0;
}

/* Fills vals[0..k) with distinct numbers from lo..hi. */
static void pick_values(int *vals, int k, int lo, int hi)
{
    static const int special[] = {-1000, 0, 1000};
    int m = hi - lo + 1, i, r;
    int *pool = malloc((size_t)m * sizeof *pool);

    for (i = 0; i < m; i++)
        pool[i] = lo + i;
    rnd_shuffle(pool, m);
    for (i = 0; i < k; i++)
        vals[i] = pool[i];
    free(pool);
    if (lo == -1000)
        for (i = 0; i < 3; i++)
            if (rnd_chance(30) && !has(vals, k, special[i])) {
                r = (int)rnd(0, k - 1);
                vals[r] = special[i];
            }
}

/* Makes counts[a] equal to counts[b] for two random places a != b,
 * lowering the larger one so that the total doesn't grow. */
static void clash(int *counts, int k)
{
    int a = (int)rnd(0, k - 1), b = (int)rnd(0, k - 2);

    if (b >= a)
        b++;
    if (counts[a] < counts[b])
        counts[b] = counts[a];
    else
        counts[a] = counts[b];
}

void gen(int small)
{
    int vals[44], counts[44], c5[5] = {1, 2, 3, 4, 5};
    int *arr = malloc(1000 * sizeof *arr);
    int n = 0, k = 0, i, j, shape, wide = 1;

    if (small) {
        shape = (int)rnd(0, 2);
        if (shape == 0) {
            n = (int)rnd(1, 10);
            for (i = 0; i < n; i++)
                arr[i] = (int)rnd(-3, 3);
        } else {
            k = (int)rnd(1, 4);
            rnd_shuffle(c5, 5);
            for (i = 0; i < k; i++)
                counts[i] = c5[i];
            if (k >= 2 && rnd_chance(40))
                clash(counts, k);
            wide = shape == 2;
        }
    } else {
        shape = (int)rnd(0, 3);
        if (shape <= 1) {
            k = 44;
            for (i = 0; i < k; i++)
                counts[i] = i + 1;
            if (shape == 1)
                clash(counts, k);
        } else if (shape == 2) {
            k = (int)rnd(1, 10);
            for (i = 0; i < k; i++)
                do
                    counts[i] = (int)rnd(1, 1000 / k);
                while (has(counts, i, counts[i]));
            if (k >= 2 && rnd_chance(50))
                clash(counts, k);
        } else if (rnd_chance(50)) {
            k = 1;
            counts[0] = 1000;
        } else {
            n = 1000;
            for (i = 0; i < n; i++)
                arr[i] = (int)rnd(-1000, 1000);
        }
    }
    if (k > 0) {
        pick_values(vals, k, wide ? -1000 : -4, wide ? 1000 : 4);
        for (i = 0; i < k; i++)
            for (j = 0; j < counts[i]; j++)
                arr[n++] = vals[i];
    }
    rnd_shuffle(arr, n);
    put_ints(arr, n);
    free(arr);
}
