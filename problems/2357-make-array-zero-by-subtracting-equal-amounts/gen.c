/* Small cases: up to 8 entries drawn from a palette of a few values in
 * 0..9, so that zeros, repeats and all-equal arrays come up often (brute.c
 * is exponential in the values). Full-size cases have 100 entries, in one
 * of five shapes picked by the seed: every value 1..100 once (answer 100),
 * every value 0..99 once (99), one value repeated, all zeros, or random
 * values in 0..100. */
void gen(int small)
{
    int nums[100], palette[4];
    int n, k, shape, i, j;

    if (small) {
        n = (int)rnd(1, 8);
        k = (int)rnd(1, 4);
        for (i = 0; i < k; i++)
            palette[i] = rnd_chance(30) ? 0 : (int)rnd(1, 9);
        for (i = 0; i < n; i++) {
            j = (int)rnd(0, k - 1);
            nums[i] = palette[j];
        }
        put_ints(nums, n);
        return;
    }
    n = 100;
    shape = (int)rnd(0, 4);
    k = (int)rnd(1, 100);
    for (i = 0; i < n; i++) {
        if (shape == 0)
            nums[i] = i + 1;
        else if (shape == 1)
            nums[i] = i;
        else if (shape == 2)
            nums[i] = k;
        else if (shape == 3)
            nums[i] = 0;
        else
            nums[i] = (int)rnd(0, 100);
    }
    rnd_shuffle(nums, n);
    put_ints(nums, n);
}
