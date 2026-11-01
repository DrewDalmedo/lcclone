/* A 0/1 matrix with its 1s scattered at random places in each row. The
 * seed picks the shape: rows of random tallies (from a narrow range when
 * small, so that ties are common), tallies rising down the rows so that
 * the last row wins, all rows equal, or (full size only, since small ones
 * often come out so anyway) all 0s. Small matrices are at most 6 x 6;
 * full-size ones are 100 x 100, 100 x 1 or 1 x 100. */
void gen(int small)
{
    int m, n, shape, lo, hi, i, j, t;
    int *tally, *a, *row;

    if (small) {
        m = (int)rnd(1, 6);
        n = (int)rnd(1, 6);
    } else {
        shape = (int)rnd(0, 2);
        m = shape == 2 ? 1 : 100;
        n = shape == 1 ? 1 : 100;
    }
    tally = malloc((size_t)m * sizeof *tally);
    a = malloc((size_t)m * n * sizeof *a);
    shape = (int)rnd(0, small ? 4 : 5);
    if (shape <= 1) { /* random, in a narrow range when small */
        lo = small ? (int)rnd(0, n) : 0;
        hi = small ? (int)rnd(lo, lo + 1 > n ? n : lo + 1) : n;
        for (i = 0; i < m; i++)
            tally[i] = (int)rnd(lo, hi);
    } else if (shape == 2) { /* rising: the last row has the most */
        for (i = 0; i < m; i++)
            tally[i] = (int)((long long)n * (i + 1) / m);
    } else if (shape == 3) { /* all equal */
        t = (int)rnd(0, n);
        for (i = 0; i < m; i++)
            tally[i] = t;
    } else if (shape == 4) { /* any tally at all */
        for (i = 0; i < m; i++)
            tally[i] = (int)rnd(0, n);
    } else { /* all 0s */
        for (i = 0; i < m; i++)
            tally[i] = 0;
    }
    for (i = 0; i < m; i++) {
        row = a + (size_t)i * n;
        for (j = 0; j < n; j++)
            row[j] = j < tally[i];
        rnd_shuffle(row, n);
    }
    put_int_grid(a, m, n);
    free(tally);
    free(a);
}
