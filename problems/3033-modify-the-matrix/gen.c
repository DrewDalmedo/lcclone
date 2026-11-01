/* A grid with blanks (-1). Small cases: 2 to 4 rows and columns, values
 * mostly from 0..5 (now and then up to 100), with 15 to 50 percent of the
 * cells blank, so that zeros, several blanks in one column and a column's
 * largest value below its blanks come up often. Full-size cases, in a
 * shape the seed picks:
 *   0: 50 x 50, values 0..100, 15 to 50 percent blank;
 *   1: 50 x 50, each column all blank but one cell, often the bottom one;
 *   2: 2 x 50 or 50 x 2;
 *   3: 50 x 50 with no blank at all.
 * A column left all blank gets one value back, often in the bottom row, so
 * that every column keeps a number that is not -1. */
void gen(int small)
{
    int shape = small ? 0 : (int)(gen_seed() % 4);
    int m = 50, n = 50, top, blank, i, j, r;
    int *a;

    if (small) {
        m = (int)rnd(2, 4);
        n = (int)rnd(2, 4);
    } else if (shape == 2) {
        m = rnd_chance(50) ? 2 : 50;
        n = m == 2 ? 50 : 2;
    }
    top = (small && rnd_chance(75)) ? 5 : 100;
    if (shape == 1)
        blank = 100;
    else if (shape == 3)
        blank = 0;
    else
        blank = (int)rnd(15, 50);
    a = malloc((size_t)m * n * sizeof *a);
    for (i = 0; i < m * n; i++)
        a[i] = rnd_chance(blank) ? -1 : (int)rnd(0, top);
    for (j = 0; j < n; j++) {
        for (i = 0; i < m && a[i * n + j] == -1; i++)
            ;
        if (i == m) {
            r = rnd_chance(50) ? m - 1 : (int)rnd(0, m - 1);
            a[r * n + j] = (int)rnd(0, top);
        }
    }
    put_int_grid(a, m, n);
    free(a);
}
