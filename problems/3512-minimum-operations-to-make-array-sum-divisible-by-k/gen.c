/* Random piles and a random k. Small cases have a few low piles; k is
 * often at most the total, otherwise anything up to 30, so totals below k,
 * k = 1 and exact multiples come up often. Full-size cases have 1000
 * piles: all 1000, all from 900 to 1000, or anything from 1 to 1000, with
 * k up to 100 (often exactly 100). Either way the seed sometimes picks k
 * among the divisors of the total. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 6) : 1000, i, k, d;
    int top = small ? (int)rnd(1, 10) : 1000;
    int shape = (int)rnd(0, 2), total = 0, nd = 0, divs[100];
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++) {
        if (small)
            nums[i] = (int)rnd(1, top);
        else if (shape == 0)
            nums[i] = 1000;
        else
            nums[i] = (int)rnd(shape == 1 ? 900 : 1, 1000);
        total += nums[i];
    }
    if (rnd_chance(20)) {
        for (d = 1; d <= 100 && d <= total; d++)
            if (total % d == 0)
                divs[nd++] = d;
        k = divs[rnd(0, nd - 1)];
    } else if (small && rnd_chance(60)) {
        k = (int)rnd(1, total < 30 ? total : 30);
    } else if (small) {
        k = (int)rnd(1, 30);
    } else {
        k = rnd_chance(25) ? 100 : (int)rnd(1, 100);
    }
    put_ints(nums, n);
    put_int(k);
    free(nums);
}
