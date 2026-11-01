/* Values that often share a bit count, given in scrambled order. Small
 * cases hold up to 8 values from 0 to 20, or now and then from a list of
 * awkward ones (powers of two, 8191 with the most bits, its 12-bit
 * neighbours, 10^4, values that fool a key of bits * 1000, 1024 or 8192).
 * Full-size cases hold 500 values: uniform over 0..10^4, all with one bit
 * count in decreasing order, few distinct values, or one value repeated. */
static int popcount(int v)
{
    int b = 0;

    while (v) {
        v &= v - 1;
        b++;
    }
    return b;
}

static int by_desc(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x < y) - (x > y);
}

void gen(int small)
{
    static const int odd[] = {0, 1, 2, 3, 7, 63, 1024, 2048, 3072, 4096,
                              6143, 8189, 8190, 8191, 8192, 10000};
    int n = small ? (int)rnd(1, 8) : 500, i, shape, b, v, pool[6];
    int *arr = malloc((size_t)n * sizeof *arr);

    if (small) {
        for (i = 0; i < n; i++) {
            if (rnd_chance(25))
                arr[i] = odd[rnd(0, 15)];
            else
                arr[i] = (int)rnd(0, 20);
        }
    } else {
        shape = (int)rnd(0, 3);
        if (shape == 0) {
            for (i = 0; i < n; i++)
                arr[i] = (int)rnd(0, 10000);
        } else if (shape == 1) {
            b = (int)rnd(2, 8);
            for (i = 0; i < n; i++) {
                do
                    v = (int)rnd(0, 10000);
                while (popcount(v) != b);
                arr[i] = v;
            }
            qsort(arr, (size_t)n, sizeof *arr, by_desc);
        } else if (shape == 2) {
            for (i = 0; i < 6; i++)
                pool[i] = (int)rnd(0, 10000);
            for (i = 0; i < n; i++)
                arr[i] = pool[rnd(0, 5)];
        } else {
            v = (int)rnd(0, 10000);
            for (i = 0; i < n; i++)
                arr[i] = v;
        }
    }
    put_ints(arr, n);
    free(arr);
}
