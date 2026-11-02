/* Values from the whole range, only negatives, only positives, or a narrow
 * window somewhere in the range, which makes repeats and ties likely (half
 * of the small cases use one). Small cases have 3 to 8 values, full-size
 * ones 100. */
void gen(int small)
{
    int nums[100];
    int n = small ? (int)rnd(3, 8) : 100, shape = (int)rnd(0, small ? 5 : 3);
    int lo = -100, hi = 100, width, i;

    if (shape == 1) {
        hi = -1;
    } else if (shape == 2) {
        lo = 1;
    } else if (shape >= 3) {
        width = (int)rnd(0, small ? 5 : 20);
        lo = (int)rnd(-100, 100 - width);
        hi = lo + width;
    }
    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(lo, hi);
    put_ints(nums, n);
}
