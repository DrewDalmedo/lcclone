/* Slow and obviously right: try every sequence of k picks (n^k of them,
 * fine for the small random cases), raising the picked counter by 1 after
 * each pick and undoing it on the way back, and keep the best total. */
static int best_total(int *nums, int n, int moves_left)
{
    int best = 0, i, earned, t;

    if (moves_left == 0)
        return 0;
    for (i = 0; i < n; i++) {
        earned = nums[i];
        nums[i]++;
        t = earned + best_total(nums, n, moves_left - 1);
        nums[i]--;
        if (t > best)
            best = t;
    }
    return best;
}

int maximizeSum(int *nums, int numsSize, int k)
{
    return best_total(nums, numsSize, k);
}
