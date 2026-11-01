/* Small cases: up to 8 values from a narrow range and a small k, so that
 * pairs and repeats are common. Full size: 200 values, in a shape the seed
 * picks: any values and any k; only the values x and x + k (thousands of
 * pairs, up to 10000); values up to 10 with k up to 9; or values up to 50
 * with k of 50 or more, so that there is no pair at all. */
void gen(int small)
{
    int nums[200];
    int n = small ? (int)rnd(1, 8) : 200, shape, top, k, x, i;

    if (small) {
        top = (int)rnd(2, 8);
        k = (int)rnd(1, top - 1);
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, top);
    } else {
        shape = (int)rnd(0, 3);
        k = shape == 2 ? (int)rnd(1, 9) : shape == 3 ? (int)rnd(50, 99)
                                                     : (int)rnd(1, 99);
        x = (int)rnd(1, 100 - k);
        for (i = 0; i < n; i++) {
            if (shape == 0)
                nums[i] = (int)rnd(1, 100);
            else if (shape == 1)
                nums[i] = rnd_chance(50) ? x : x + k;
            else if (shape == 2)
                nums[i] = (int)rnd(1, 10);
            else
                nums[i] = (int)rnd(1, 50);
        }
    }
    put_ints(nums, n);
    put_int(k);
}
