/* Small cases: a few transactions from -6 to 6, so that answers of 1, dips
 * to exactly 0 or -1, and ties for the lowest balance come up often.
 * Full-size cases: 100 transactions, of a shape the seed picks: any
 * values; mostly withdrawals; a long slide down then a climb back above
 * the start (the lowest balance in the middle); no deposits at all (the
 * lowest balance at the very end); or every transaction -100, the largest
 * answer. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, shape = (int)rnd(0, 4), i, v;
    int nums[100];

    for (i = 0; i < n; i++) {
        if (small)
            v = (int)rnd(-6, 6);
        else if (shape == 0)
            v = (int)rnd(-100, 100);
        else if (shape == 1)
            v = (int)rnd(-100, 30);
        else if (shape == 2)
            v = i < n / 2 ? (int)rnd(-100, 10) : (int)rnd(-10, 100);
        else if (shape == 3)
            v = (int)rnd(-100, 0);
        else
            v = -100;
        nums[i] = v;
    }
    put_ints(nums, n);
}
