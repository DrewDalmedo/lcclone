/* Small cases: 1 to 6 children and 0 to 6 cookies, sizes from 1 to 6 so
 * that equal sizes and ties come up often, or now and then sizes at the
 * top of the range, up to 2^31 - 1. Full-size ones have up to 3 * 10^4
 * of each, in a shape the seed picks: random sizes over the whole range,
 * both arrays sorted, both sorted the other way (slow for a quicksort
 * that pivots on an end), every size the same, every cookie too small
 * (slow for scanning all cookies for each child), or random lengths. */
static void fill_random(int *a, int n, long long lo, long long hi)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = (int)rnd(lo, hi);
}

/* Increasing sizes, from below 70000 to at most 2.1 * 10^9. */
static void fill_sorted(int *a, int n, int down)
{
    int i;
    long long v;

    for (i = 0; i < n; i++) {
        v = 1 + (long long)i * 70000 + rnd(0, 69999);
        a[down ? n - 1 - i : i] = (int)v;
    }
}

void gen(int small)
{
    int gn, sn, shape, v;
    long long lo, hi;
    int *g, *s;

    if (small) {
        gn = (int)rnd(1, 6);
        sn = (int)rnd(0, 6);
    } else {
        gn = 30000;
        sn = 30000;
    }
    g = malloc(((size_t)gn + 1) * sizeof *g);
    s = malloc(((size_t)sn + 1) * sizeof *s);
    if (small) {
        if (rnd_chance(15)) {
            lo = 2147483643;
            hi = 2147483647;
        } else {
            lo = 1;
            hi = 6;
        }
        fill_random(g, gn, lo, hi);
        fill_random(s, sn, lo, hi);
    } else {
        shape = (int)(gen_seed() % 6);
        switch (shape) {
        case 0: /* random sizes over the whole range */
            fill_random(g, gn, 1, 2147483647);
            fill_random(s, sn, 1, 2147483647);
            break;
        case 1: /* both sorted, smallest first */
        case 2: /* both sorted, largest first */
            fill_sorted(g, gn, shape == 2);
            fill_sorted(s, sn, shape == 2);
            break;
        case 3: /* every child and every cookie the same size */
            v = (int)rnd(1, 2147483647);
            fill_random(g, gn, v, v);
            fill_random(s, sn, v, v);
            break;
        case 4: /* every cookie smaller than every child: no one content */
            fill_random(g, gn, 1000000001, 2147483647);
            fill_random(s, sn, 1, 1000000000);
            break;
        default: /* random lengths and sizes, so either side may run out */
            gn = (int)rnd(1, 30000);
            sn = (int)rnd(0, 30000);
            fill_random(g, gn, 1, 2000000000);
            fill_random(s, sn, 1, 2000000000);
            break;
        }
    }
    put_ints(g, gn);
    put_ints(s, sn);
    free(g);
    free(s);
}
