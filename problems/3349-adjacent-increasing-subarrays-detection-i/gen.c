/* Values that mostly climb, with drops and repeats, so that increasing
 * runs of many lengths come up; the seed picks how often they rise. Often
 * two rising halves of k are planted somewhere (at the far end, the slow
 * place for a scan from the left), sometimes with one flaw in them. */
static void climb(int *a, int len, int top)
{
    int maxstep = (2 * top) / (len > 1 ? len - 1 : 1), j, v, d;

    if (maxstep > 3 * top / 10 + 3)
        maxstep = 3 * top / 10 + 3;
    v = (int)rnd(-top, top - (len - 1) * maxstep);
    for (j = 0; j < len; j++) {
        if (j > 0) {
            d = (int)rnd(1, maxstep);
            v += d;
        }
        a[j] = v;
    }
}

void gen(int small)
{
    int n, i, v, r, d, k, a, flaw;
    int rise = (int)rnd(50, 95);
    int top = small ? 9 : 1000;
    int step = small ? 3 : 60;
    int *nums;

    if (small)
        n = rnd_chance(15) ? (int)rnd(2, 3) : (int)rnd(4, 8);
    else
        n = 100;
    if (rnd_chance(10)) {
        k = 1;
    } else if (small) {
        k = (int)rnd(n / 2 > 1 ? 2 : 1, n / 2);
    } else {
        r = (int)rnd(0, 2);
        if (r == 0)
            k = (int)rnd(2, 10);
        else if (r == 1)
            k = (int)rnd(11, 25);
        else
            k = (int)rnd(26, 50);
    }
    nums = malloc((size_t)n * sizeof *nums);
    v = (int)rnd(-top, top);
    for (i = 0; i < n; i++) {
        if (i > 0) {
            r = (int)rnd(1, 100);
            if (r <= rise) {
                d = (int)rnd(1, step);
                v += d;
            } else if (r <= rise + (100 - rise) / 3) {
                /* a repeat */
            } else {
                d = (int)rnd(0, 2 * step);
                v -= d;
            }
            if (v > top || v < -top)
                v = (int)rnd(-top, 0);
        }
        nums[i] = v;
    }
    if (rnd_chance(50)) {
        a = rnd_chance(50) ? n - 2 * k : (int)rnd(0, n - 2 * k);
        climb(nums + a, k, top);
        climb(nums + a + k, k, top);
        if (k > 1 && rnd_chance(50)) {
            /* one repeat inside one of the halves */
            flaw = (int)rnd(1, 2 * k - 1);
            if (flaw != k)
                nums[a + flaw] = nums[a + flaw - 1];
        }
    }
    put_ints(nums, n);
    put_int(k);
    free(nums);
}
