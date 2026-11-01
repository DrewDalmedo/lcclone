/* Values from -10 to 10 in most small cases, else from -100 to 100. The
 * array is built from left to right, and each new element may be chosen to
 * make the element before it a pivot (when that one is even and the needed
 * value is in range), or, behind a nonzero even one, sometimes to make the
 * neighbours add up to minus its half, or, behind an odd one, to hit its
 * half rounded toward zero or rounded down; neither of those two traps must
 * count. The seed picks how often: never, sometimes or nearly always, so
 * pivots also come side by side. A few cases are all zeros. Small cases
 * have 3 to 8 elements; full-size ones mostly 100. */
void gen(int small)
{
    int n = small ? (int)rnd(3, 8) : rnd_chance(75) ? 100 : (int)rnd(3, 100);
    int top = small && !rnd_chance(20) ? 10 : 100;
    int plant = (int)rnd(0, 2) * 45, zeros = rnd_chance(4);
    int nums[100], i, a, b, c, half, r;

    for (i = 0; i < n; i++) {
        nums[i] = (int)rnd(-top, top);
        if (zeros)
            nums[i] = 0;
        if (i < 2 || zeros)
            continue;
        a = nums[i - 2];
        b = nums[i - 1];
        r = (int)rnd(0, 99);
        if (r >= plant)
            continue;
        if (b % 2 == 0)
            half = b != 0 && rnd_chance(25) ? -b / 2 : b / 2; /* wrong sign */
        else if (rnd_chance(50) || b > 0)
            half = b / 2; /* rounded toward zero */
        else
            half = (b - 1) / 2; /* rounded down */
        c = half - a;
        if (c >= -100 && c <= 100)
            nums[i] = c;
    }
    put_ints(nums, n);
}
