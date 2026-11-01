/* Reference solution: one pass over the maximal strictly increasing runs.
 * A working stretch either lies inside one run of at least 2k elements,
 * or takes the last k elements of one run and the first k of the next. */
bool hasIncreasingSubarrays(int *nums, int numsSize, int k)
{
    int prev = 0, cur = 1, i, pair;

    for (i = 1; i <= numsSize; i++) {
        if (i < numsSize && nums[i] > nums[i - 1]) {
            cur++;
            continue;
        }
        /* The run of cur elements ends here; prev is the run before it. */
        pair = prev < cur ? prev : cur;
        if (pair >= k || cur / 2 >= k)
            return true;
        prev = cur;
        cur = 1;
    }
    return false;
}
