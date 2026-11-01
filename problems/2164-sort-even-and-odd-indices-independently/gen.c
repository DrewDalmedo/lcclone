/* Small cases: 1 to 9 numbers from 1 to 6, so ties come up often.
 * Full-size ones: 99 or 100 numbers from 1 to 100, with a 1 and a 100
 * among them; half of them start with the even indices descending and the
 * odd ones ascending, the opposite of the result. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 9) : (int)rnd(99, 100);
    int top = small ? 6 : 100;
    int nums[100], i, j, k, t;

    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(1, top);
    if (!small) {
        k = (int)rnd(0, n - 1);
        nums[k] = 1;
        k = (int)rnd(0, n - 1);
        nums[k] = 100;
        if (rnd_chance(50))
            for (i = 2; i < n; i++)
                for (j = i; j >= 2; j -= 2) {
                    if (j % 2 == 0 ? nums[j - 2] >= nums[j]
                                   : nums[j - 2] <= nums[j])
                        break;
                    t = nums[j];
                    nums[j] = nums[j - 2];
                    nums[j - 2] = t;
                }
    }
    put_ints(nums, n);
}
