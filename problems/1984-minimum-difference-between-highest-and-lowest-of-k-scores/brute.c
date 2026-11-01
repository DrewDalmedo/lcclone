/* Slow and obviously right: try every group of k positions, tracking the
 * lowest and highest score picked so far. (Exponential: it is only run on
 * the small random cases, which have at most 8 scores.) */
static int best;

static void pick(const int *nums, int n, int from, int left, int lo, int hi)
{
    int i, l, h;

    if (left == 0) {
        if (hi - lo < best)
            best = hi - lo;
        return;
    }
    for (i = from; i + left <= n; i++) {
        l = nums[i] < lo ? nums[i] : lo;
        h = nums[i] > hi ? nums[i] : hi;
        pick(nums, n, i + 1, left - 1, l, h);
    }
}

int minimumDifference(int *nums, int numsSize, int k)
{
    best = INT_MAX;
    pick(nums, numsSize, 0, k, INT_MAX, INT_MIN);
    return best;
}
