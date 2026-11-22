/* Random values and a random window length k. Full-size cases have 10^5
 * values, in a shape the seed picks: random, with k = n / 2 (the most work
 * for adding up every window anew); all -10^4, so that a best sum that
 * starts at 0 is wrong; all 10^4 with k = n, the largest sum (10^9);
 * increasing, with k = n / 2, so that the best window is the last; or
 * random with a random k. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100000, i;
    int k = (int)rnd(1, n);
    int top = small ? 10 : 10000;
    int shape = small ? 4 : (int)(gen_seed() % 5);
    int *nums = malloc((size_t)n * sizeof *nums);

    if (shape != 4)
        k = shape == 2 ? n : n / 2;
    for (i = 0; i < n; i++) {
        if (shape == 1)
            nums[i] = -10000;
        else if (shape == 2)
            nums[i] = 10000;
        else if (shape == 3)
            nums[i] = (int)((long long)i * 20001 / n) - 10000;
        else
            nums[i] = (int)rnd(-top, top);
    }
    put_ints(nums, n);
    put_int(k);
    free(nums);
}
