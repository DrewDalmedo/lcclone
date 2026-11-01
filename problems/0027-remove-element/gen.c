/* Random values and a val that usually appears in nums, often many times;
 * now and then a val that doesn't appear, possibly above 50. Small cases
 * use a few small values, so repeats and runs of val come up often. */
void gen(int small)
{
    int n = small ? (int)rnd(0, 8) : 100, i, val;
    int top = small ? (int)rnd(0, 5) : (rnd_chance(50) ? 50 : 9);
    int share = (int)rnd(0, 60);
    int nums[100];

    if (rnd_chance(10))
        val = (int)rnd(51, 100);
    else
        val = (int)rnd(0, top);
    for (i = 0; i < n; i++)
        nums[i] = rnd_chance(share) && val <= 50 ? val : (int)rnd(0, top);
    put_ints(nums, n);
    put_int(val);
}
