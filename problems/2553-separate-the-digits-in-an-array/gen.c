/* Small cases: a few numbers of 1 to 5 digits, often with zeros inside
 * or at the end, and now and then 100000, the only 6-digit value. Full
 * size: 1000 numbers, in a shape the seed picks: all 100000 (the longest
 * answer, 6000 digits); any values; or numbers of mixed lengths. */
static int number(int small)
{
    int len, d, v, k;

    if (rnd_chance(small ? 8 : 10))
        return 100000;
    len = (int)rnd(1, 5);
    v = (int)rnd(1, 9);
    for (k = 1; k < len; k++) {
        if (rnd_chance(35))
            d = 0;
        else
            d = (int)rnd(0, 9);
        v = v * 10 + d;
    }
    return v;
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 6) : 1000;
    int shape = small ? 2 : (int)rnd(0, 2), i;
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++) {
        if (shape == 0)
            nums[i] = 100000;
        else if (shape == 1)
            nums[i] = (int)rnd(1, 100000);
        else
            nums[i] = number(small);
    }
    put_ints(nums, n);
    free(nums);
}
