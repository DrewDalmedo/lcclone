/* Arrays made of runs of one value each, where a run of v holds v copies,
 * one more or one fewer, all shuffled, so that lucky values and near
 * misses are both common (the same value may get two runs). Some cases
 * are plain random values instead. Small cases have at most 12 entries
 * of 1 to 6; full-size ones have 500 entries, and some are the extremes:
 * 500 copies of 500, or 1 to 31 each lucky. */
void gen(int small)
{
    int cap = small ? 12 : 500, top = small ? 6 : 500;
    int *arr = malloc((size_t)cap * sizeof *arr);
    int n = small ? (int)rnd(1, cap) : cap;
    int shape = (int)rnd(0, small ? 3 : 5);
    int len = 0, v, lim, c, d, i;

    if (shape == 0) {
        /* Plain random values. */
        for (len = 0; len < n; len++)
            arr[len] = (int)rnd(1, top);
    } else if (shape == 4) {
        /* 500 copies of 500, sometimes with one replaced by a 1. */
        for (len = 0; len < n; len++)
            arr[len] = 500;
        if (rnd_chance(50))
            arr[rnd(0, n - 1)] = 1;
    } else if (shape == 5) {
        /* Each of 1 to 31 lucky (496 entries), and four more entries of
         * another value, which may spoil one of them. */
        for (v = 1; v <= 31; v++)
            for (i = 0; i < v; i++)
                arr[len++] = v;
        v = (int)rnd(1, 40);
        while (len < n)
            arr[len++] = v;
    } else {
        /* Runs of near-lucky length; small values are likelier, so that
         * several runs fit. */
        while (len < n) {
            lim = (int)rnd(1, top);
            v = (int)rnd(1, lim);
            d = (int)rnd(-1, 1);
            c = v + d;
            if (c < 1)
                c = 1;
            if (c > n - len)
                c = n - len;
            for (i = 0; i < c; i++)
                arr[len++] = v;
        }
    }
    rnd_shuffle(arr, n);
    put_ints(arr, n);
    free(arr);
}
