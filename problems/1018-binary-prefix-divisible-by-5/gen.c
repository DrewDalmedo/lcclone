/* The seed picks a shape: random bits; a run of zeros, then random bits;
 * all ones; mostly zeros (long runs where the counter only doubles); and
 * mostly ones. Small cases usually have 1 to 10 bits, and sometimes 60 to
 * 100, past what a 64-bit integer holds; full-size ones have 10^5. */
void gen(int small)
{
    int n, shape, lead, i;
    int *nums;

    if (!small)
        n = 100000;
    else if (rnd_chance(80))
        n = (int)rnd(1, 10);
    else
        n = (int)rnd(60, 100);
    shape = (int)rnd(0, 4);
    lead = shape == 1 ? (int)rnd(0, n) : 0;
    nums = malloc((size_t)n * sizeof *nums);
    for (i = 0; i < n; i++) {
        if (i < lead)
            nums[i] = 0;
        else if (shape == 2)
            nums[i] = 1;
        else if (shape == 3)
            nums[i] = rnd_chance(5);
        else if (shape == 4)
            nums[i] = rnd_chance(90);
        else
            nums[i] = (int)rnd(0, 1);
    }
    put_ints(nums, n);
    free(nums);
}
