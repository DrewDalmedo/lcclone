/* The seed picks where the first match is: nowhere, or some position up
 * to 27 (the largest a digit sum can be). Every position before it gets a
 * value whose digit sum differs from the position; the positions after
 * it get any value, so later matches come up too. Small cases are mostly
 * short with small values, now and then long enough to reach position
 * 27; full-size ones have 100 values up to 1000, with no match at all or
 * the first one at position 27 as two of their shapes. */
static int digit_sum(int v)
{
    int s = 0;

    for (; v > 0; v /= 10)
        s += v % 10;
    return s;
}

/* A value from 0 to 1000 with digit sum t, for 0 <= t <= 27. */
static int with_sum(int t)
{
    int a, b;

    if (t == 1 && rnd_chance(30))
        return 1000;
    a = (int)rnd(t > 18 ? t - 18 : 0, t < 9 ? t : 9);
    b = (int)rnd(t - a > 9 ? t - a - 9 : 0, t - a < 9 ? t - a : 9);
    return a * 100 + b * 10 + (t - a - b);
}

/* A value from 0 to top (at least 1) whose digit sum is not t. */
static int without_sum(int t, int top)
{
    int v;

    do
        v = (int)rnd(0, top);
    while (digit_sum(v) == t);
    return v;
}

void gen(int small)
{
    int nums[100];
    int n, m, top, last, shape, i;

    if (small) {
        n = rnd_chance(20) ? (int)rnd(26, 30) : (int)rnd(1, 8);
        top = rnd_chance(50) ? 30 : 1000;
        last = n - 1 < 27 ? n - 1 : 27;
        m = rnd_chance(25) ? -1 : (int)rnd(0, last);
    } else {
        n = 100;
        top = 1000;
        shape = (int)rnd(0, 2);
        m = shape == 0 ? -1 : shape == 1 ? 27 : (int)rnd(0, 27);
    }
    for (i = 0; i < n; i++) {
        if (m < 0 || i < m)
            nums[i] = without_sum(i, top);
        else if (i == m)
            nums[i] = with_sum(i);
        else
            nums[i] = (int)rnd(0, top);
    }
    put_ints(nums, n);
}
