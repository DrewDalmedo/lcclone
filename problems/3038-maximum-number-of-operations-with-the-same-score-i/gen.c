/* Pairs that add up to one score S, up to a break the seed places: no
 * break (every pair adds up to S), a break at the last pair, a break at a
 * random pair, or no pattern at all (every value random). The pair at the
 * break never adds up to S; pairs past it do half the time, to catch
 * solutions that don't stop at the first break. An odd length adds a lone
 * last value. Small cases: up to 9 values from 1 to 6, so equal sums come
 * up often. Full size: mostly 99 or 100 values from 1 to 1000. */

/* Two values from 1 to top that add up to s (2 <= s <= 2 * top). */
static void matching_pair(int s, int top, int *a, int *b)
{
    int lo = s - top > 1 ? s - top : 1, hi = s - 1 < top ? s - 1 : top;

    *a = (int)rnd(lo, hi);
    *b = s - *a;
}

void gen(int small)
{
    int nums[100];
    int top = small ? 6 : 1000;
    int n, pairs, s, how, brk, i, a, b;

    if (small)
        n = (int)rnd(2, 9);
    else
        n = rnd_chance(70) ? (int)rnd(99, 100) : (int)rnd(2, 100);
    pairs = n / 2;
    s = (int)rnd(2, 2 * top);
    how = (int)rnd(0, 3);
    if (how == 0 || (how == 1 && pairs == 1))
        brk = pairs;
    else if (how == 1)
        brk = pairs - 1;
    else
        brk = (int)rnd(1, pairs);
    for (i = 0; i < pairs; i++) {
        if (how == 3) {
            a = (int)rnd(1, top);
            b = (int)rnd(1, top);
        } else if (i < brk || (i > brk && rnd_chance(50))) {
            matching_pair(s, top, &a, &b);
        } else {
            a = (int)rnd(1, top);
            b = (int)rnd(1, top);
            if (i == brk && a + b == s)
                b = b < top ? b + 1 : b - 1;
        }
        nums[2 * i] = a;
        nums[2 * i + 1] = b;
    }
    if (n % 2)
        nums[n - 1] = (int)rnd(1, top);
    put_ints(nums, n);
}
