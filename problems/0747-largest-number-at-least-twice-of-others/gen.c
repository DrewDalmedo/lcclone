/* Each case picks a runner-up r and a largest value m > r, often within
 * one of 2r (2r - 1, 2r or 2r + 1, as far as the range allows), so that
 * the "exactly double" boundary comes up a lot. m goes at a random index,
 * r at another one, and the rest are at most r, often equal to r or 0.
 * Small cases have up to 8 elements, usually with a runner-up of at most
 * 10; full-size ones have 50, with a runner-up anywhere from 0 to 99. */
void gen(int small)
{
    int nums[50];
    int n = small ? (int)rnd(2, 8) : 50;
    int r = small && rnd_chance(75) ? (int)rnd(0, 10) : (int)rnd(0, 99);
    int m, d, pm, pr, i, v, kind;

    if (rnd_chance(60)) {
        d = (int)rnd(-1, 1);
        m = 2 * r + d;
        if (m <= r)
            m = r + 1;
        if (m > 100)
            m = 100;
    } else {
        m = (int)rnd(r + 1, 100);
    }
    kind = (int)rnd(0, 2);
    for (i = 0; i < n; i++) {
        if (kind == 0)
            v = (int)rnd(0, r);
        else if (kind == 1)
            v = rnd_chance(50) ? r : 0;
        else
            v = (int)rnd(r / 2, r);
        nums[i] = v;
    }
    pm = (int)rnd(0, n - 1);
    do
        pr = (int)rnd(0, n - 1);
    while (pr == pm);
    nums[pm] = m;
    nums[pr] = r;
    put_ints(nums, n);
}
