/* Numbers of every length from 1 to 6 digits (6 means 10^5 itself), often
 * right at a boundary, such as 9, 10, 99 or 100, and sometimes repeated.
 * Small cases have up to 8 numbers, full-size ones 500. */
static int number(void)
{
    static const int low[] = {1, 10, 100, 1000, 10000, 100000};
    int len = (int)rnd(1, 6), pick = (int)rnd(0, 3);
    int lo = low[len - 1], hi = len == 6 ? 100000 : low[len] - 1;

    if (pick == 0)
        return lo;
    if (pick == 1)
        return hi;
    return (int)rnd(lo, hi);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 500, i;
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++)
        nums[i] = i > 0 && rnd_chance(15) ? nums[rnd(0, i - 1)] : number();
    put_ints(nums, n);
    free(nums);
}
