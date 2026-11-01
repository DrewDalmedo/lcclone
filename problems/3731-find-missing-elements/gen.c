/* Distinct values from lo to hi: lo and hi themselves, plus some of the
 * values between them, all shuffled so the ends can sit anywhere.
 *
 * Small cases hold up to 8 values over a range of at most 12 integers,
 * which may start at 1 or end at 100, so a full run (nothing missing) is
 * common; now and then the range is all of 1 to 100, with a long answer.
 * Full-size ones pick by seed: all of 1 to 100 (nothing missing), 1 and
 * 100 alone (98 missing), 1 and 100 with a random number of values
 * between, or a random range holding a random share of its values. */
void gen(int small)
{
    int pool[100], nums[100], n, lo, hi, w, shape, cnt, v, i;

    shape = (int)rnd(0, 7);
    if (small) {
        if (shape == 0) {
            lo = 1;
            hi = 100;
        } else {
            w = (int)rnd(1, 11);
            if (shape == 1)
                lo = 1;
            else if (shape == 2)
                lo = 100 - w;
            else
                lo = (int)rnd(1, 100 - w);
            hi = lo + w;
        }
        n = (int)rnd(2, hi - lo + 1 < 8 ? hi - lo + 1 : 8);
    } else {
        lo = 1;
        hi = 100;
        if (shape <= 1) {
            n = 100;
        } else if (shape == 2) {
            n = 2;
        } else if (shape <= 4) {
            n = (int)rnd(3, 99);
        } else {
            lo = (int)rnd(1, 99);
            hi = (int)rnd(lo + 1, 100);
            n = (int)rnd(2, hi - lo + 1);
        }
    }
    cnt = 0;
    for (v = lo + 1; v < hi; v++)
        pool[cnt++] = v;
    rnd_shuffle(pool, cnt);
    nums[0] = lo;
    nums[1] = hi;
    for (i = 2; i < n; i++)
        nums[i] = pool[i - 2];
    rnd_shuffle(nums, n);
    put_ints(nums, n);
}
