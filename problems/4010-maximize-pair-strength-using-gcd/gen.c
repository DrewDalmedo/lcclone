/* Random values, often all multiples of one factor, so that pairs share
 * divisors; sometimes every value is the same. Small cases have 2 to 8
 * values of at most 24; full size ones have 2000 values of at most 10^5. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 2000, i;
    int top = small ? (int)rnd(2, 24) : 100000;
    int factor = rnd_chance(50) ? 1 : (int)rnd(2, small ? 4 : 50);
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++)
        nums[i] = factor * (int)rnd(1, top / factor > 0 ? top / factor : 1);
    if (rnd_chance(5))
        for (i = 1; i < n; i++)
            nums[i] = nums[0];
    put_ints(nums, n);
    free(nums);
}
