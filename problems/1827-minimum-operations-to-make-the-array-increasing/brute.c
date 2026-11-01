/* Slow and obviously right: try every possible final height for every
 * stack. cost[v] is the fewest moves for the stacks so far when the last
 * of them ends at height v; the next stack can end at any w >= its height
 * with w > v, which costs w - nums[i] more. Heights above M + n, with
 * M = max(nums), never help: capping a staircase b at min(b[i], M + i)
 * keeps it climbing and not below nums, and costs no more. */
int minOperations(int *nums, int numsSize)
{
    const long long none = 1LL << 60;
    long long *cost, *next, low;
    int top = 0, i, v;

    for (i = 0; i < numsSize; i++)
        if (nums[i] > top)
            top = nums[i];
    top += numsSize;
    cost = malloc((size_t)(top + 1) * sizeof *cost);
    next = malloc((size_t)(top + 1) * sizeof *next);
    for (v = 0; v <= top; v++)
        cost[v] = v >= nums[0] ? v - nums[0] : none;
    for (i = 1; i < numsSize; i++) {
        low = none; /* the smallest cost[u] over all u < v */
        for (v = 0; v <= top; v++) {
            next[v] = v >= nums[i] && low < none ? low + (v - nums[i]) : none;
            if (cost[v] < low)
                low = cost[v];
        }
        memcpy(cost, next, (size_t)(top + 1) * sizeof *cost);
    }
    low = none;
    for (v = 0; v <= top; v++)
        if (cost[v] < low)
            low = cost[v];
    free(cost);
    free(next);
    return (int)low;
}
