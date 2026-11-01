/* An array of 2 to 8 numbers when small, 500 at full size. The seed picks
 * the shape: any numbers (from a narrow range when small, so that pairs
 * and zeros come up often); odd numbers only, which never pair, with a 0
 * or two dropped in (a lone 0, or one at each end, at full size); odd
 * numbers v each next to the near miss 2v - 1 or 2v + 1, which halves to
 * v with integer division; or odd numbers with one planted pair, put in
 * the last two places at full size. Full-size odd numbers are distinct
 * (but for the near misses), so a search over all pairs finds nothing
 * early. */
static int small_odd(void)
{
    int v = (int)rnd(-7, 7);

    return 2 * v + 1;
}

void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 500, shape = (int)rnd(0, 4), i, v, d;
    int *a = malloc((size_t)n * sizeof *a), *odd = NULL;

    if (small) {
        for (i = 0; i < n; i++)
            a[i] = small_odd();
    } else {
        /* 500 distinct odd numbers out of the 1000 from -999 to 999 */
        odd = malloc(1000 * sizeof *odd);
        for (i = 0; i < 1000; i++)
            odd[i] = 2 * i - 999;
        rnd_shuffle(odd, 1000);
        for (i = 0; i < n; i++)
            a[i] = odd[i];
        free(odd);
    }
    if (shape == 0) { /* any numbers */
        for (i = 0; i < n; i++)
            a[i] = small ? (int)rnd(-12, 12) : (int)rnd(-1000, 1000);
    } else if (shape == 1) { /* odd numbers, and maybe a 0 or two */
        if (small) {
            for (i = 0; i < n; i++)
                if (rnd_chance(20))
                    a[i] = 0;
        } else if (rnd_chance(50)) { /* a lone 0 */
            i = (int)rnd(0, n - 1);
            a[i] = 0;
        } else { /* two 0s, as far apart as can be */
            a[0] = 0;
            a[n - 1] = 0;
        }
    } else if (shape == 2) { /* near misses: v next to 2v - 1 or 2v + 1 */
        for (i = 0; i + 1 < n; i += 2) {
            v = small ? small_odd() : 2 * (int)rnd(-250, 249) + 1;
            d = rnd_chance(50) ? 1 : -1;
            a[i] = v;
            a[i + 1] = 2 * v + d;
        }
        if (small)
            rnd_shuffle(a, n);
    } else { /* odd numbers with one planted pair v, 2v */
        v = small ? (int)rnd(-6, 6) : (int)rnd(1, 500);
        if (!small && rnd_chance(50))
            v = -v;
        i = small ? (int)rnd(0, n - 1) : n - 2;
        d = small ? (int)rnd(0, n - 2) : n - 1;
        if (d >= i && small)
            d++;
        if (rnd_chance(50)) {
            a[i] = v;
            a[d] = 2 * v;
        } else {
            a[i] = 2 * v;
            a[d] = v;
        }
    }
    put_ints(a, n);
    free(a);
}
