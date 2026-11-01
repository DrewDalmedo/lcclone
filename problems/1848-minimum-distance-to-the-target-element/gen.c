/* A row of values, a target that occurs in it, and a start index. Small
 * cases have 1 to 8 values from three in a row (1 to 3, or 9998 to 10000
 * at the upper bound), so target occurs often and ties come up. Full-size
 * ones have 1000 values up to 10^4; the seed picks target only once, at
 * the far end from a start at one end (the answer is 999), target at a
 * few random places, or target only at one end with start anywhere. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000;
    int *nums = malloc((size_t)n * sizeof *nums);
    int shape = (int)rnd(0, 2), target, start, lo, i, k, v, copies;

    if (small) {
        lo = rnd_chance(25) ? 9998 : 1;
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(lo, lo + 2);
        k = (int)rnd(0, n - 1);
        target = nums[k];
        start = (int)rnd(0, n - 1);
    } else {
        target = (int)rnd(1, 10000);
        for (i = 0; i < n; i++) {
            v = (int)rnd(1, 9999);
            nums[i] = v >= target ? v + 1 : v;
        }
        if (shape == 0) {
            start = rnd_chance(50) ? 0 : n - 1;
            nums[n - 1 - start] = target;
        } else if (shape == 1) {
            copies = (int)rnd(2, 5);
            for (i = 0; i < copies; i++) {
                k = (int)rnd(0, n - 1);
                nums[k] = target;
            }
            start = (int)rnd(0, n - 1);
        } else {
            k = rnd_chance(50) ? 0 : n - 1;
            nums[k] = target;
            start = (int)rnd(0, n - 1);
        }
    }
    put_ints(nums, n);
    put_int(target);
    put_int(start);
    free(nums);
}
