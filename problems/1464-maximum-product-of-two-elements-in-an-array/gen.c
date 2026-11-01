/* Small cases have 2 to 8 entries, full-size ones 500. The seed picks the
 * shape: uniform values (from 1 to 3 or 12 when small, so values repeat
 * and ties come up); mostly ones, so the answer is often 0; values from
 * 1, 2, 999 and 1000, so the largest is often repeated and the answer
 * reaches 998001; or uniform values sorted up or down, so the two largest
 * come last or first. */
void gen(int small)
{
    static const int edge[] = {1, 2, 999, 1000};
    int n = small ? (int)rnd(2, 8) : 500, i, j, k, t, shape, top;
    int *nums = malloc((size_t)n * sizeof *nums);

    shape = (int)rnd(0, 3);
    top = 1000;
    if (small)
        top = rnd_chance(50) ? 3 : 12;
    for (i = 0; i < n; i++) {
        if (shape == 1 && rnd_chance(75)) {
            nums[i] = 1;
        } else if (shape == 2) {
            k = (int)rnd(0, 3);
            nums[i] = edge[k];
        } else {
            nums[i] = (int)rnd(1, top);
        }
    }
    if (shape == 3) {
        k = rnd_chance(50);
        for (i = 1; i < n; i++)
            for (j = i; j > 0 && (k ? nums[j - 1] > nums[j]
                                    : nums[j - 1] < nums[j]); j--) {
                t = nums[j];
                nums[j] = nums[j - 1];
                nums[j - 1] = t;
            }
    }
    put_ints(nums, n);
    free(nums);
}
