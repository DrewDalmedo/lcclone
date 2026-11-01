/* Distinct random values. Half the time, plant a lucky number: pick a
 * cell, gather the values of its row and column, and put the m-th
 * smallest there, the m - 1 smaller ones in the rest of its column and the
 * larger ones in the rest of its row. */
void gen(int small)
{
    int m = small ? (int)rnd(1, 4) : 50, n = small ? (int)rnd(1, 4) : 50;
    int top = small ? 20 : 100000, i, j, k, v, r, c, cnt;
    char *used = calloc((size_t)top + 1, 1);
    int *a = malloc((size_t)m * n * sizeof *a);
    int *vals = malloc((size_t)(m + n) * sizeof *vals);

    for (k = 0; k < m * n; k++) {
        do
            v = (int)rnd(1, top);
        while (used[v]);
        used[v] = 1;
        a[k] = v;
    }
    if (rnd_chance(50)) {
        r = (int)rnd(0, m - 1);
        c = (int)rnd(0, n - 1);
        cnt = 0;
        for (i = 0; i < m; i++)
            vals[cnt++] = a[i * n + c];
        for (j = 0; j < n; j++)
            if (j != c)
                vals[cnt++] = a[r * n + j];
        /* sort the m + n - 1 values */
        for (i = 1; i < cnt; i++)
            for (k = i; k > 0 && vals[k - 1] > vals[k]; k--) {
                v = vals[k];
                vals[k] = vals[k - 1];
                vals[k - 1] = v;
            }
        rnd_shuffle(vals, m - 1);
        rnd_shuffle(vals + m, n - 1);
        a[r * n + c] = vals[m - 1];
        k = 0;
        for (i = 0; i < m; i++)
            if (i != r)
                a[i * n + c] = vals[k++];
        k = m;
        for (j = 0; j < n; j++)
            if (j != c)
                a[r * n + j] = vals[k++];
    }
    put_int_grid(a, m, n);
    free(used);
    free(a);
    free(vals);
}
