/* nums and divisors. The seed picks the shape: small values with many
 * shared factors, so that scores tie often; nums built as multiples of a
 * few chosen divisors, with values up to 10^9; or divisors all larger than
 * every num, so that every score is 0. Small cases have at most 8 nums and
 * 6 divisors; full-size ones have 1000 of each. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000;
    int k = small ? (int)rnd(1, 6) : 1000;
    int shape = (int)rnd(0, 3), i, top, d, mult;
    int *nums = malloc((size_t)n * sizeof *nums);
    int *divs = malloc((size_t)k * sizeof *divs);

    if (shape <= 1) { /* small values with shared factors */
        top = small ? (shape == 0 ? 12 : 30) : 1000;
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, top);
        for (i = 0; i < k; i++)
            divs[i] = (int)rnd(1, small ? top / 2 : 100);
    } else if (shape == 2) { /* multiples of a few divisors, up to 10^9 */
        for (i = 0; i < k; i++)
            divs[i] = (int)rnd(1, small ? 20 : 100000);
        for (i = 0; i < n; i++) {
            d = divs[rnd(0, k - 1)];
            mult = (int)rnd(1, 1000000000 / d);
            nums[i] = d * mult;
        }
    } else { /* every divisor larger than every num: all scores 0 */
        top = small ? 10 : 500000000;
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, top);
        for (i = 0; i < k; i++)
            divs[i] = (int)rnd(top + 1, small ? 20 : 1000000000);
    }
    put_ints(nums, n);
    put_ints(divs, k);
    free(nums);
    free(divs);
}
