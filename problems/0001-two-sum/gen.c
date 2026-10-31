/* Exactly one pair may add up to the target: draw the values and a pair,
 * and draw again while another pair has the same sum. */

static int one_pair(const int *nums, int n, long long target)
{
    int i, k, found = 0;

    for (i = 0; i < n; i++)
        for (k = i + 1; k < n; k++)
            if ((long long)nums[i] + nums[k] == target && ++found > 1)
                return 0;
    return found == 1;
}

void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 10000, i, x, y;
    int top = small ? 20 : 1000000000;
    int *nums = malloc((size_t)n * sizeof *nums);
    long long target;

    do {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(-top, top);
        x = (int)rnd(0, n - 1);
        do
            y = (int)rnd(0, n - 1);
        while (y == x);
        target = (long long)nums[x] + nums[y];
    } while (target < -1000000000 || target > 1000000000 ||
             !one_pair(nums, n, target));
    put_ints(nums, n);
    put_int(target);
    free(nums);
}
