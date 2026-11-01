/* A shuffled permutation of 1..n, with each entry replaced by a random
 * value from 1..n at a chance drawn per case: from no change at all (an
 * empty answer) to every entry random. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100000, i;
    int percent = (int)rnd(0, 100);
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++)
        nums[i] = i + 1;
    rnd_shuffle(nums, n);
    for (i = 0; i < n; i++)
        if (rnd_chance(percent))
            nums[i] = (int)rnd(1, n);
    put_ints(nums, n);
    free(nums);
}
