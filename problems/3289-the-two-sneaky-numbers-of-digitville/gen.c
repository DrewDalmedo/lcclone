/* Each of 0..n-1 once, plus two different values of that range again,
 * shuffled. Small cases have a small n, so 0 and n - 1 are often among
 * the two. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 10) : 100, i, a, b;
    int *nums = malloc((size_t)(n + 2) * sizeof *nums);

    for (i = 0; i < n; i++)
        nums[i] = i;
    a = (int)rnd(0, n - 1);
    do
        b = (int)rnd(0, n - 1);
    while (b == a);
    nums[n] = a;
    nums[n + 1] = b;
    rnd_shuffle(nums, n + 2);
    put_ints(nums, n + 2);
    free(nums);
}
