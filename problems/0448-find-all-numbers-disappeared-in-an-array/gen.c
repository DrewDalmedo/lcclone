/* A shuffled permutation of 1..n, with each entry replaced by a random
 * value from 1..n at a chance drawn per case: from no change at all (an
 * empty answer) to every entry random. Full-size cases have n = 10^5, in a
 * shape the seed picks: a permutation (nothing missing, and each number
 * must be found); every entry random; every entry n (the longest answer,
 * and no search for a missing number stops early); or a random chance. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100000, i;
    int percent = (int)rnd(0, 100), shape = small ? -1 : (int)(gen_seed() % 4);
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++)
        nums[i] = i + 1;
    rnd_shuffle(nums, n);
    if (shape == 0)
        percent = 0;
    else if (shape == 1)
        percent = 100;
    for (i = 0; i < n; i++)
        if (rnd_chance(percent))
            nums[i] = (int)rnd(1, n);
    if (shape == 2)
        for (i = 0; i < n; i++)
            nums[i] = n;
    put_ints(nums, n);
    free(nums);
}
