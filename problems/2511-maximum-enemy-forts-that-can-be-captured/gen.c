/* A map of -1, 0 and 1. Small cases have up to 10 positions: mostly
 * markers (1 or -1, often alternating) between runs of 0 to 3 enemy
 * forts, sometimes any cells at all. Full-size ones have 1000 positions;
 * the seed picks a few forts and empty positions scattered among long
 * runs of enemy forts, a dense random map, a map with only one kind of
 * marker besides the enemy forts (answer 0), or one long route between a
 * fort and an empty position near the two ends, with same-kind markers
 * past its ends and now and then a marker cutting it. */
/* 0 with a chance of zero_pct in 100, else 1 or -1, as likely. */
static int cell(int zero_pct)
{
    if (rnd_chance(zero_pct))
        return 0;
    return rnd_chance(50) ? 1 : -1;
}

void gen(int small)
{
    static int forts[1000];
    int n = small ? (int)rnd(1, 10) : 1000, shape = (int)rnd(0, 3);
    int i, k, at, sign;

    if (small && rnd_chance(25)) {  /* any cells */
        for (i = 0; i < n; i++)
            forts[i] = cell(50);
    } else if (small) {             /* markers between short runs of 0 */
        sign = rnd_chance(50) ? 1 : -1;
        i = (int)rnd(0, 2);
        memset(forts, 0, sizeof forts);
        while (i < n) {
            forts[i] = sign;
            k = (int)rnd(0, 3);
            i += k + 1;
            if (rnd_chance(65))
                sign = -sign;
        }
    } else if (shape == 0) {        /* sparse markers, long runs */
        memset(forts, 0, sizeof forts);
        k = (int)rnd(2, 40);
        for (i = 0; i < k; i++) {
            at = (int)rnd(0, n - 1);
            forts[at] = rnd_chance(50) ? 1 : -1;
        }
    } else if (shape == 1) {        /* dense */
        for (i = 0; i < n; i++)
            forts[i] = cell(60);
    } else if (shape == 2) {        /* only one kind of marker */
        sign = rnd_chance(50) ? 1 : -1;
        for (i = 0; i < n; i++)
            forts[i] = rnd_chance(95) ? 0 : sign;
    } else {                        /* one far route, decoys around it */
        memset(forts, 0, sizeof forts);
        sign = rnd_chance(50) ? 1 : -1;
        at = (int)rnd(0, 3);
        forts[at] = sign;
        k = (int)rnd(n - 4, n - 1);
        forts[k] = -sign;
        /* Same-kind markers outside the route: the runs past its ends. */
        for (i = 0; i < at; i++)
            forts[i] = rnd_chance(50) ? sign : 0;
        for (i = k + 1; i < n; i++)
            forts[i] = rnd_chance(50) ? -sign : 0;
        /* Now and then cut the route with a marker, splitting it in two. */
        if (rnd_chance(50)) {
            i = (int)rnd(at + 1, k - 1);
            forts[i] = rnd_chance(50) ? sign : -sign;
        }
    }
    put_ints(forts, n);
}
