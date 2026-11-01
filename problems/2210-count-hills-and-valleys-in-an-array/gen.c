/* Small cases have 3 to 8 elements of heights up to a cap of 2 to 4, so
 * that plateaus, equal heights and ends come up often. Full-size ones
 * have 100 elements. The seed picks the shape: random heights from a small
 * or the full range, heights that zigzag between two values with runs of
 * random length (most turns), every element equal, or heights that never
 * go down (no turns). */
void gen(int small)
{
    int n = small ? (int)rnd(3, 8) : 100, i, run, v, lo, hi;
    int top = small ? (int)rnd(2, 4) : rnd_chance(50) ? 100 : (int)rnd(2, 5);
    int shape = (int)rnd(0, 4);
    int nums[100];

    if (shape <= 1) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, top);
    } else if (shape == 2) {
        lo = (int)rnd(1, small ? top - 1 : 99);
        hi = (int)rnd(lo + 1, small ? top : 100);
        v = rnd_chance(50) ? lo : hi;
        for (i = 0; i < n; ) {
            for (run = (int)rnd(1, 3); run > 0 && i < n; run--)
                nums[i++] = v;
            v = v == lo ? hi : lo;
        }
    } else if (shape == 3) {
        v = (int)rnd(1, small ? top : 100);
        for (i = 0; i < n; i++)
            nums[i] = v;
    } else {
        v = (int)rnd(1, small ? 3 : 20);
        for (i = 0; i < n; i++) {
            if (rnd_chance(50) && v < 100)
                v++;
            nums[i] = v;
        }
    }
    put_ints(nums, n);
}
