/* Distinct values. The seed picks the parities: all odd, all even,
 * exactly one odd among evens, exactly one even among odds, or a random
 * mix. Small cases have 1 to 8 values up to 20. Full-size ones use
 * values up to 100: every odd or every even one (50 values), one value
 * of the other parity with all 50 of the first (51), or all of 1..100
 * in random order. */
static int pick(int *used, int top, int odd)
{
    int v;

    do {
        v = (int)rnd(1, top);
    } while (used[v] || (odd >= 0 && v % 2 != odd));
    used[v] = 1;
    return v;
}

void gen(int small)
{
    int nums[100], used[101] = {0};
    int top = small ? 20 : 100, shape = (int)rnd(0, 4), n, i, k, odd;

    if (small)
        n = (int)rnd(1, 8);
    else
        n = shape == 4 ? 100 : shape <= 1 ? 50 : 51;
    k = (int)rnd(0, n - 1);
    for (i = 0; i < n; i++) {
        if (shape == 0 || shape == 1)
            odd = shape == 0;
        else if (shape == 2)
            odd = i == k;
        else if (shape == 3)
            odd = i != k;
        else
            odd = -1;
        nums[i] = pick(used, top, odd);
    }
    put_ints(nums, n);
}
