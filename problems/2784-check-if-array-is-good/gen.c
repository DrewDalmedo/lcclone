/* Starts from a good array 1, 2, ..., k-1, k, k with k = m - 1, then by
 * the seed's shape keeps it (good), or spoils it in one way: one element
 * changed at random; a pair x-d, x+d turned into x, x (same sum, same
 * largest value, same length); the top value k left once or made three
 * times; one element too many or too few; or all values random. Then it
 * shuffles. Small cases: up to 8 elements, values near the length (200
 * now and then); full size: mostly 100 elements, values up to 200. A
 * single element is any value. */
void gen(int small)
{
    int nums[101];
    int m = small ? (int)rnd(1, 8) : (rnd_chance(70) ? 100 : (int)rnd(1, 100));
    int cap = small ? 8 : 100;
    int top = small ? m + 1 : 200;
    int shape = (int)rnd(0, 7), k = m - 1, i, x, d, at, v;

    if (m == 1) {
        nums[0] = rnd_chance(50) ? 1 : (int)rnd(1, small ? 3 : 200);
        put_ints(nums, 1);
        return;
    }
    for (i = 0; i < k; i++)
        nums[i] = i + 1;
    nums[k] = k;
    if (shape == 2) {
        /* one element changed (sometimes to the value it had) */
        at = (int)rnd(0, m - 1);
        v = small && rnd_chance(10) ? 200 : (int)rnd(1, top);
        nums[at] = v;
    } else if (shape == 3 && k >= 3) {
        /* x-d and x+d (x+d may be k) become two more copies of x */
        x = (int)rnd(2, k - 1);
        d = (int)rnd(1, x - 1 < k - x ? x - 1 : k - x);
        nums[x - d - 1] = x;
        nums[x + d - 1] = x;
    } else if (shape == 4 && k >= 2) {
        /* k once (a smaller value doubled) or k three times */
        at = (int)rnd(0, k - 2);
        if (rnd_chance(50))
            nums[k] = nums[at];
        else
            nums[at] = k;
    } else if (shape == 5) {
        /* one element too many, or too few */
        if (m < cap && rnd_chance(50)) {
            v = (int)rnd(1, k);
            nums[m++] = v;
        } else {
            m--;
        }
    } else if (shape == 6) {
        for (i = 0; i < m; i++)
            nums[i] = (int)rnd(1, small ? m : top);
    }
    /* shapes 0, 1 and 7 (and the ones that don't apply): good */
    rnd_shuffle(nums, m);
    put_ints(nums, m);
}
