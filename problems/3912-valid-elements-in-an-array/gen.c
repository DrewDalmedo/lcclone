/* Small cases: 1 to 8 values, mostly from 1 to a top of 1 to 5, so that
 * ties and repeated maxima are common; now and then from 1 to 100.
 * Full size: 100 values, in a shape the seed picks: random; 1 to 100
 * rising or falling, or rising to 100 and then falling, all distinct
 * (every post is seen); two tall ends around low values (only the ends
 * are seen); random values with 100 placed at 3 to 10 spots; or values
 * from 1 to 3 only. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, i, v, k, shape, lo, hi, top;
    int nums[100], left[100], right[100], nl = 0, nr = 0;

    if (small) {
        top = rnd_chance(80) ? (int)rnd(1, 5) : 100;
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, top);
        put_ints(nums, n);
        return;
    }
    shape = (int)rnd(0, 6);
    if (shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, 100);
    } else if (shape == 1) {
        for (i = 0; i < n; i++)
            nums[i] = i + 1;
    } else if (shape == 2) {
        for (i = 0; i < n; i++)
            nums[i] = n - i;
    } else if (shape == 3) {
        for (v = 1; v <= 99; v++) {
            if (rnd_chance(50))
                left[nl++] = v;
            else
                right[nr++] = v;
        }
        for (i = 0; i < nl; i++)
            nums[i] = left[i];
        nums[nl] = 100;
        for (i = 0; i < nr; i++)
            nums[nl + 1 + i] = right[nr - 1 - i];
    } else if (shape == 4) {
        lo = (int)rnd(90, 100);
        hi = (int)rnd(90, 100);
        nums[0] = lo;
        nums[n - 1] = hi;
        if (hi < lo)
            lo = hi;
        for (i = 1; i < n - 1; i++)
            nums[i] = (int)rnd(1, lo - 1);
    } else if (shape == 5) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, 99);
        k = (int)rnd(3, 10);
        while (k-- > 0) {
            i = (int)rnd(0, n - 1);
            nums[i] = 100;
        }
    } else {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, 3);
    }
    put_ints(nums, n);
}
