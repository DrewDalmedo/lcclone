/* The seed picks a shape: values from 0 to 50; values from 0 to 7, which
 * share their few bits; mostly zeros with single bits here and there; or
 * 32 first and 16 last with values below 16 between them, where only the
 * whole array reaches k = 48. Outside the last shape, k is 0, any value,
 * or close to the OR of the whole array, so that the answer is -1 about
 * as often as it is a length. Small cases have 1 to 8 elements, full-size
 * ones 50. */
void gen(int small)
{
    int nums[50];
    int n = small ? (int)rnd(1, 8) : 50, shape = (int)rnd(0, 3);
    int all = 0, k, r, i;

    if (shape == 3 && n < 2)
        n = 2;
    for (i = 0; i < n; i++) {
        if (shape == 0)
            nums[i] = (int)rnd(0, 50);
        else if (shape == 1)
            nums[i] = (int)rnd(0, 7);
        else if (shape == 2)
            nums[i] = rnd_chance(50) ? 0 : 1 << (int)rnd(0, 5);
        else
            nums[i] = i == 0 ? 32 : i == n - 1 ? 16 : (int)rnd(0, 15);
        all |= nums[i];
    }
    r = (int)rnd(0, 9);
    if (shape == 3)
        k = (int)rnd(40, 48);
    else if (r == 0)
        k = 0;
    else if (r <= 3)
        k = (int)rnd(0, 63);
    else
        k = all + (int)rnd(-3, 2);
    if (k < 0)
        k = 0;
    if (k > 63)
        k = 63;
    put_ints(nums, n);
    put_int(k);
}
