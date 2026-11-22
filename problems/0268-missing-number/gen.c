/* The numbers 0 to n but one, shuffled (half the time), sorted, or sorted
 * in reverse. Small cases have n from 1 to 9, and the missing one is 0 a
 * quarter of the time, n a quarter of the time, and otherwise any of 0 to
 * n. Full-size ones have n = 10^4, and the seed picks the missing one: 0,
 * n (the slowest for trying 0, 1, 2, ... in turn), or any. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 9) : 10000, miss, order, i, k = 0, t;
    int *nums = malloc((size_t)n * sizeof *nums);

    switch (small ? rnd(0, 3) : (long long)(gen_seed() % 3)) {
    case 0:
        miss = 0;
        break;
    case 1:
        miss = n;
        break;
    default:
        miss = (int)rnd(0, n);
    }
    for (i = 0; i <= n; i++)
        if (i != miss)
            nums[k++] = i;
    order = (int)rnd(0, 3);
    if (order <= 1)
        rnd_shuffle(nums, n);
    else if (order == 3)
        for (i = 0; i < n / 2; i++) {
            t = nums[i];
            nums[i] = nums[n - 1 - i];
            nums[n - 1 - i] = t;
        }
    put_ints(nums, n);
    free(nums);
}
