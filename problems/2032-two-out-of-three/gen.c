/* Each list draws its values from a random range of its own, so lists
 * overlap a lot, a little or not at all, and a narrow range repeats
 * values within a list. Small cases use values up to 10, sometimes moved
 * up next to 100. */
static void put_list(int len, int top, int shift)
{
    int nums[100], i, lo = (int)rnd(1, top), hi = (int)rnd(lo, top);

    for (i = 0; i < len; i++)
        nums[i] = (int)rnd(lo, hi) + shift;
    put_ints(nums, len);
}

void gen(int small)
{
    int top = small ? (int)rnd(2, 10) : 100;
    int shift = small && rnd_chance(25) ? 100 - top : 0, k;

    for (k = 0; k < 3; k++)
        put_list(small ? (int)rnd(1, 5) : 100, top, shift);
}
