/* Random rides. Small cases have 1 to 4 rides of each kind with values up
 * to 3 or up to 12, so ties and duplicates come up often. Full-size cases
 * have up to 100 rides of each kind (often exactly 100), and the seed
 * picks how the values spread:
 *   - opening times and durations anywhere in 1..1000;
 *   - short rides (durations up to 30), so waiting for an opening matters;
 *   - long rides that open early (durations from 500), so the first ride
 *     usually ends after the second one opens;
 *   - every value 1000, or every value 1. */
static void fill(int *a, int count, int lo, int hi)
{
    int i;

    for (i = 0; i < count; i++)
        a[i] = (int)rnd(lo, hi);
}

void gen(int small)
{
    int ls[100], ld[100], ws[100], wd[100];
    int n, m, shape, top;
    int slo = 1, shi = 1000, dlo = 1, dhi = 1000;

    if (small) {
        n = (int)rnd(1, 4);
        m = (int)rnd(1, 4);
        top = rnd_chance(30) ? 3 : 12;
        shi = dhi = top;
    } else {
        n = rnd_chance(60) ? 100 : (int)rnd(1, 100);
        m = rnd_chance(60) ? 100 : (int)rnd(1, 100);
        shape = (int)rnd(0, 4);
        if (shape == 1) {
            dhi = 30;
        } else if (shape == 2) {
            shi = 300;
            dlo = 500;
        } else if (shape == 3) {
            slo = dlo = 1000;
        } else if (shape == 4) {
            shi = dhi = 1;
        }
    }
    fill(ls, n, slo, shi);
    fill(ld, n, dlo, dhi);
    fill(ws, m, slo, shi);
    fill(wd, m, dlo, dhi);
    put_ints(ls, n);
    put_ints(ld, n);
    put_ints(ws, m);
    put_ints(wd, m);
}
