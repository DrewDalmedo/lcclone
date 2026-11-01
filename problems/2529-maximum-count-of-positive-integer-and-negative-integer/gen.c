/* A sorted array: a block of negatives, a block of zeros and a block of
 * positives, any of which may be empty. Two random cut points split the
 * array into the three blocks; the zero block is often dropped, and now
 * and then the whole array is negative, zero or positive, or every
 * nonzero value is -top or top. Small cases have 1 to 8 values from -3 to
 * 3, so equal values and ties between the counts come up often; full-size
 * ones have 2000 values from -2000 to 2000. */
static int cmp(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 2000, top = small ? 3 : 2000;
    int *nums = malloc((size_t)n * sizeof *nums);
    int cut1 = (int)rnd(0, n), cut2 = (int)rnd(0, n);
    int neg = cut1 < cut2 ? cut1 : cut2;
    int zeros = cut1 < cut2 ? cut2 - cut1 : cut1 - cut2;
    int extreme = rnd_chance(20), i, k;

    if (rnd_chance(30))
        zeros = 0;
    if (rnd_chance(15)) {
        /* every value negative, every value zero, or every value positive */
        k = (int)rnd(0, 2);
        neg = k == 0 ? n : 0;
        zeros = k == 1 ? n : 0;
    }
    for (i = 0; i < n; i++) {
        k = extreme ? top : (int)rnd(1, top);
        if (i < neg)
            nums[i] = -k;
        else if (i < neg + zeros)
            nums[i] = 0;
        else
            nums[i] = k;
    }
    qsort(nums, (size_t)n, sizeof *nums, cmp);
    put_ints(nums, n);
    free(nums);
}
