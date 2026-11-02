/* An array and a window width k. The width is 1, the whole length, or
 * anything between, as the seed picks. Values come from a range the seed
 * picks: half the time a few small ones from 0 up (so repeats, and -1,
 * are common), else 44 to 50 or all of 0 to 50. Small cases have 1 to 8
 * elements; full-size ones have 50. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 50, i, k, lo, hi, shape;
    int *nums = malloc((size_t)n * sizeof *nums);

    shape = (int)rnd(0, 3);
    lo = shape == 2 ? 44 : 0;
    hi = shape <= 1 ? (int)rnd(1, 5) : 50;
    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(lo, hi);
    shape = (int)rnd(0, 3);
    if (shape == 0)
        k = 1;
    else if (shape == 1)
        k = n;
    else
        k = (int)rnd(1, n);
    put_ints(nums, n);
    put_int(k);
    free(nums);
}
