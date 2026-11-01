/* Starts from a Latin square on 1..n: the square whose row i is 0..n-1
 * shifted by i, with its rows, its columns and its values put in random
 * orders. Then, as the seed picks: leaves it alone (twice as likely),
 * changes one cell to another value, swaps two cells of a row (the rows
 * stay complete, two columns break), swaps two cells of a column, or
 * fills every cell at random. Small cases have n from 1 to 5. Full-size
 * ones have n = 100, put a changed cell in one of the last rows, and
 * make the repeated and the missing value both larger than 64 there. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 5) : 100;
    int *g = malloc((size_t)n * (size_t)n * sizeof *g);
    int rp[100], cp[100], sym[100];
    int shape = (int)rnd(0, 5), i, j, r, c, a, b, t, old, v;

    for (i = 0; i < n; i++) {
        rp[i] = i;
        cp[i] = i;
        sym[i] = i + 1;
    }
    rnd_shuffle(rp, n);
    rnd_shuffle(cp, n);
    rnd_shuffle(sym, n);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            g[i * n + j] = sym[(rp[i] + cp[j]) % n];
    if (n > 1 && shape == 2) {
        if (small) {
            r = (int)rnd(0, n - 1);
            c = (int)rnd(0, n - 1);
            old = g[r * n + c];
            v = (int)rnd(1, n - 1);
            if (v >= old)
                v++;
        } else {
            r = (int)rnd(n - 3, n - 1);
            old = (int)rnd(65, n);
            for (c = 0; g[r * n + c] != old; c++)
                ;
            v = (int)rnd(65, n - 1);
            if (v >= old)
                v++;
        }
        g[r * n + c] = v;
    } else if (n > 1 && shape == 3) {
        r = (int)rnd(0, n - 1);
        a = (int)rnd(0, n - 1);
        b = (int)rnd(0, n - 2);
        if (b >= a)
            b++;
        t = g[r * n + a];
        g[r * n + a] = g[r * n + b];
        g[r * n + b] = t;
    } else if (n > 1 && shape == 4) {
        c = (int)rnd(0, n - 1);
        a = (int)rnd(0, n - 1);
        b = (int)rnd(0, n - 2);
        if (b >= a)
            b++;
        t = g[a * n + c];
        g[a * n + c] = g[b * n + c];
        g[b * n + c] = t;
    } else if (shape == 5) {
        for (i = 0; i < n * n; i++)
            g[i] = (int)rnd(1, n);
    }
    put_int_grid(g, n, n);
    free(g);
}
