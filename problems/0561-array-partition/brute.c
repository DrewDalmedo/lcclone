/* Slow and obviously right: try every way of pairing up the elements (the
 * first unpaired element goes with each other unpaired one in turn, and
 * the rest are paired up the same way), and keep the best total of the
 * pairs' smaller values. */
static int best_total(const int *nums, int n, char *used)
{
    int first = 0, j, total, best = INT_MIN;

    while (first < n && used[first])
        first++;
    if (first == n)
        return 0;
    used[first] = 1;
    for (j = first + 1; j < n; j++) {
        if (used[j])
            continue;
        used[j] = 1;
        total = (nums[first] < nums[j] ? nums[first] : nums[j]) +
                best_total(nums, n, used);
        if (total > best)
            best = total;
        used[j] = 0;
    }
    used[first] = 0;
    return best;
}

int arrayPairSum(int *nums, int numsSize)
{
    char *used = calloc((size_t)numsSize, 1);
    int best = best_total(nums, numsSize, used);

    free(used);
    return best;
}
