/* A sorted array: random values from a range, counted per value and then
 * written out in increasing order. Small cases use a narrow range, now
 * and then next to -100 or 100, so long runs of repeats come up often. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 30000, i, v, at = 0;
    int lo, hi, count[201] = {0};
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small) {
        int width = (int)rnd(0, 4);

        if (rnd_chance(15))
            lo = -100;
        else if (rnd_chance(15))
            lo = 100 - width;
        else
            lo = (int)rnd(-5, 5 - width);
        hi = lo + width;
    } else {
        lo = (int)rnd(-100, 0);
        hi = (int)rnd(0, 100);
    }
    for (i = 0; i < n; i++)
        count[rnd(lo, hi) + 100]++;
    for (v = 0; v < 201; v++)
        while (count[v]-- > 0)
            nums[at++] = v - 100;
    put_ints(nums, n);
    free(nums);
}
