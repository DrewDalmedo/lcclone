/* Small cases have up to 8 numbers, often all from 1..3 and now and then
 * all equal, so repeated values and XOR totals that cancel come up often.
 * Full-size cases have 12 numbers, in a shape the seed picks:
 *   0: any values from 1..20;
 *   1: any values, with 16 and 15 among them, so their OR is 31 and the
 *      sum is the largest possible, 63488;
 *   2: only two different values;
 *   3: one value repeated 12 times. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 12;
    int shape = small ? -1 : (int)rnd(0, 3);
    int top = small && rnd_chance(40) ? 3 : 20;
    int nums[12], x, y, p, q, i;

    x = (int)rnd(1, top);
    y = (int)rnd(1, top);
    if (small && rnd_chance(15))
        shape = 3;
    for (i = 0; i < n; i++) {
        if (shape == 2)
            nums[i] = rnd_chance(50) ? x : y;
        else if (shape == 3)
            nums[i] = x;
        else
            nums[i] = (int)rnd(1, top);
    }
    if (shape == 1) {
        p = (int)rnd(0, n - 1);
        q = (int)rnd(0, n - 2);
        if (q >= p)
            q++;
        nums[p] = 16;
        nums[q] = 15;
    }
    put_ints(nums, n);
}
