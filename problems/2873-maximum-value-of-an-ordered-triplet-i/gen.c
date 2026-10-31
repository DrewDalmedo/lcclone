/* Random values; small cases mostly use a narrow range, so that equal
 * values and all-negative cases come up, and sometimes the full range. */
void gen(int small)
{
    int n = small ? (int)rnd(3, 8) : 100, i;
    int top = small && rnd_chance(70) ? (int)rnd(1, 12) : 1000000;
    int nums[100];

    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(1, top);
    put_ints(nums, n);
}
