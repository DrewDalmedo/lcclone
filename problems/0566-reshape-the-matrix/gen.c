/* mat is 1 to 4 by 1 to 4 for small cases, with values from -3 to 3 half
 * the time (so repeats are common), else from -1000 to 1000; full-size ones
 * are 100 by 100 most of the time, else of a random size up to that. The
 * seed then picks the shape asked for:
 *   - most of the time, one with as many cells as mat: r a divisor of
 *     m * n with r and m * n / r both at most 300 (m by n and n by m among
 *     them);
 *   - 300 by 300, far too many cells;
 *   - r == m, or c == n, with the other one random;
 *   - r and c both random.
 * The random ones are up to 8 for small cases and up to 300 otherwise, and
 * may fit by chance. */
void gen(int small)
{
    int m, n, r, c, k, cnt = 0, top, kind, cells, lim = small ? 8 : 300;
    int *mat, *divs;

    if (small) {
        m = (int)rnd(1, 4);
        n = (int)rnd(1, 4);
    } else if (rnd_chance(60)) {
        m = 100;
        n = 100;
    } else {
        m = (int)rnd(1, 100);
        n = (int)rnd(1, 100);
    }
    cells = m * n;
    top = small && rnd_chance(50) ? 3 : 1000;
    mat = malloc((size_t)cells * sizeof *mat);
    for (k = 0; k < cells; k++)
        mat[k] = (int)rnd(-top, top);
    kind = (int)rnd(0, 9);
    if (kind <= 5) {
        divs = malloc(300 * sizeof *divs);
        for (k = 1; k <= 300 && k <= cells; k++)
            if (cells % k == 0 && cells / k <= 300)
                divs[cnt++] = k;
        r = divs[rnd(0, cnt - 1)];
        c = cells / r;
        free(divs);
    } else if (kind == 6) {
        r = 300;
        c = 300;
    } else if (kind == 7) {
        r = m;
        c = (int)rnd(1, lim);
    } else if (kind == 8) {
        r = (int)rnd(1, lim);
        c = n;
    } else {
        r = (int)rnd(1, lim);
        c = (int)rnd(1, lim);
    }
    put_int_grid(mat, m, n);
    put_int(r);
    put_int(c);
    free(mat);
}
