/* Small cases: 1 to 8 values from -6 to 6 and a target from -7 to 7, so
 * that equal values and totals equal to target come up often, with the odd
 * extreme value of -50 or 50. Full-size ones: 50 values and a target
 * anywhere in the bounds, the values either from the whole range or
 * crowded near the target's half, so that many totals land on it. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 50;
    int target = small ? (int)rnd(-7, 7) : (int)rnd(-50, 50);
    int crowd = !small && rnd_chance(50);
    int nums[50], i, lo, hi, centre;

    centre = target / 2;
    lo = small ? -6 : crowd ? (centre - 5 < -50 ? -50 : centre - 5) : -50;
    hi = small ? 6 : crowd ? (centre + 5 > 50 ? 50 : centre + 5) : 50;
    for (i = 0; i < n; i++) {
        if (small && rnd_chance(5))
            nums[i] = rnd_chance(50) ? 50 : -50;
        else
            nums[i] = (int)rnd(lo, hi);
    }
    put_ints(nums, n);
    put_int(target);
}
