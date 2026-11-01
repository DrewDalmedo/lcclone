/* An even number of values; small cases use a narrow range so that
 * duplicates and ties are common. */
void gen(int small)
{
    int n = small ? 2 * (int)rnd(1, 4) : 50, i;
    int top = small ? (int)rnd(2, 12) : 50;
    int nums[50];

    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(1, top);
    put_ints(nums, n);
}
