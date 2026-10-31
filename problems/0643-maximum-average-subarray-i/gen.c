/* Random values and a random window length k. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100000, i;
    int k = (int)rnd(1, n);
    int top = small ? 10 : 10000;
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(-top, top);
    put_ints(nums, n);
    put_int(k);
    free(nums);
}
