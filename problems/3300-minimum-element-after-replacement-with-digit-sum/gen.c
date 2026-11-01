/* Small cases: up to 8 numbers mixing tiny values, powers of ten (digit
 * sum 1, 10000 included), numbers full of nines and arbitrary ones, so
 * that the smallest number and the smallest digit sum often differ.
 * Full-size cases have 100 numbers; the seed picks arbitrary values, high
 * digit sums with the one lowest sum at the very end, or values that are
 * (nearly) all 9999. */
static int digit_sum(int x)
{
    int s = 0;

    for (; x > 0; x /= 10)
        s += x % 10;
    return s;
}

static int pick_small(void)
{
    static const int tens[] = {1, 10, 100, 1000, 10000};
    int kind = (int)rnd(0, 9);

    if (kind < 3)
        return (int)rnd(1, 10000);
    if (kind < 5)
        return (int)rnd(1, 99);
    if (kind < 6)
        return tens[rnd(0, 4)];
    if (kind < 8)
        return (int)rnd(9000, 9999);
    return (int)rnd(100, 999);
}

void gen(int small)
{
    int nums[100], n = small ? (int)rnd(1, 8) : 100, shape, i;

    shape = (int)rnd(0, 2);
    for (i = 0; i < n; i++) {
        if (small) {
            nums[i] = pick_small();
        } else if (shape == 0) {
            nums[i] = (int)rnd(1, 10000);
        } else if (shape == 1) {
            /* Sums of 25 or more, and below 25 for the last one only. */
            do
                nums[i] = (int)rnd(1, 10000);
            while ((digit_sum(nums[i]) >= 25) != (i < n - 1));
        } else {
            nums[i] = 9999;
            if (rnd_chance(3))
                nums[i] -= 1 + 1000 * (int)rnd(0, 3);
        }
    }
    put_ints(nums, n);
}
