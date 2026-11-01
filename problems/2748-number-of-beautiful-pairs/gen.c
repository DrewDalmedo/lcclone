/* A number of 1 to 4 digits that doesn't end in 0. Its leading digit is
 * drawn from leads, its units digit from units, and each middle digit is
 * 0 half the time, so values such as 105 and 1003 come up often. */
static int number(const char *leads, const char *units)
{
    int len = (int)rnd(1, 4), v, i, d;

    v = rnd_char(leads) - '0';
    if (len == 1)
        return v;
    for (i = 2; i < len; i++) {
        d = rnd_chance(50) ? 0 : (int)rnd(1, 9);
        v = v * 10 + d;
    }
    d = rnd_char(units) - '0';
    return v * 10 + d;
}

/* Small cases: 2 to 8 numbers. Full-size cases have 100; the seed picks
 * numbers as above, uniform numbers in 1..9999 that don't end in 0, all
 * starting with 1 (every pair is beautiful), or only even leading and
 * units digits (no pair is). */
void gen(int small)
{
    const char *all = "123456789";
    const char *even = "2468";
    int nums[100];
    int n, shape, q, u, i;

    if (small) {
        n = (int)rnd(2, 8);
        for (i = 0; i < n; i++)
            nums[i] = number(all, all);
        put_ints(nums, n);
        return;
    }
    n = 100;
    shape = (int)(gen_seed() % 4);
    for (i = 0; i < n; i++) {
        if (shape == 0) {
            nums[i] = number(all, all);
        } else if (shape == 1) {
            q = (int)rnd(0, 999);
            u = (int)rnd(1, 9);
            nums[i] = q * 10 + u;
        } else if (shape == 2) {
            nums[i] = number("1", all);
        } else {
            nums[i] = number(even, even);
        }
    }
    put_ints(nums, n);
}
