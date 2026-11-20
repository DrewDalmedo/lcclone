/* n numbers; at full size 100000 of them, with k = 3 for an odd seed and
 * k = n for an even one. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100000, i, k;
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(-1000000000, 1000000000);
    k = small ? (int)rnd(1, n) : gen_seed() % 2 ? 3 : n;
    put_ints(nums, n);
    put_int(k);
    free(nums);
}
