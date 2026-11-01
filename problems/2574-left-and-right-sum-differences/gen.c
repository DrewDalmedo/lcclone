/* Small cases: a few piles, usually of 1 to 9 coins so that both sides
 * often balance, sometimes up to 10^5; now and then the row reads the same
 * both ways, which puts a gap of 0 in the middle. Full size: 1000 piles,
 * in a shape the seed picks: any sizes; all 10^5 (the largest total,
 * 10^8); or 1 to 9 coins each. */
void gen(int small)
{
    int n = 1000, shape = 0, top, i;
    int *nums;

    if (small)
        n = rnd_chance(10) ? 1 : (int)rnd(2, 7);
    else
        shape = (int)rnd(0, 2);
    nums = malloc((size_t)n * sizeof *nums);

    if (small)
        top = rnd_chance(75) ? 9 : 100000;
    else
        top = shape == 2 ? 9 : 100000;
    for (i = 0; i < n; i++) {
        if (shape == 1)
            nums[i] = 100000;
        else
            nums[i] = (int)rnd(1, top);
    }
    if (small && rnd_chance(25))
        for (i = 0; i < n / 2; i++)
            nums[n - 1 - i] = nums[i];
    put_ints(nums, n);
    free(nums);
}
