/* A row of values that mostly switch between even and odd, with a break
 * (two of one parity in a row) now and then, so that chains of many
 * lengths come up. Some rows are all odd, some have random parities, and
 * some are one long chain from the first element to the last. The
 * threshold is often the largest allowed value, one of the values, or one
 * less than a value. Small cases: up to 8 values from 1 to 12; full size:
 * 100 values from 1 to 100. */
static int of_parity(int top, int odd)
{
    /* a value from 1 to top, odd or even as asked (top >= 2) */
    if (odd)
        return 2 * (int)rnd(0, (top - 1) / 2) + 1;
    return 2 * (int)rnd(1, top / 2);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100;
    int top = small ? 12 : 100;
    int shape = (int)rnd(0, 9);
    int brk = small ? 25 : (int)rnd(1, 8);
    int *nums = malloc((size_t)n * sizeof *nums);
    int i, odd, at, how, threshold;

    /* shape 0: all odd; 1: random parities; 2: one chain over the whole
     * row; otherwise alternating, with a break at a chance of brk in 100 */
    odd = shape == 2 ? 0 : (int)rnd(0, 1);
    for (i = 0; i < n; i++) {
        if (shape == 0)
            odd = 1;
        else if (shape == 1)
            odd = (int)rnd(0, 1);
        else if (i > 0 && (shape == 2 || !rnd_chance(brk)))
            odd = !odd;
        nums[i] = of_parity(top, odd);
    }
    at = (int)rnd(0, n - 1);
    how = shape == 2 ? 0 : (int)rnd(0, 3);
    if (how == 0)
        threshold = top;
    else if (how == 1)
        threshold = nums[at];
    else if (how == 2)
        threshold = nums[at] > 1 ? nums[at] - 1 : 1;
    else
        threshold = (int)rnd(1, top);
    put_ints(nums, n);
    put_int(threshold);
    free(nums);
}
