/* Reference solution: prefix sums. With pre[k] the sum of the first k
 * elements, the view of element i is pre[i + 1] - pre[start], where
 * start = max(0, i - nums[i]). */
int subarraySum(int *nums, int numsSize)
{
    int *pre = malloc(((size_t)numsSize + 1) * sizeof *pre);
    int total = 0, i, start;

    pre[0] = 0;
    for (i = 0; i < numsSize; i++)
        pre[i + 1] = pre[i] + nums[i];
    for (i = 0; i < numsSize; i++) {
        start = i - nums[i] > 0 ? i - nums[i] : 0;
        total += pre[i + 1] - pre[start];
    }
    free(pre);
    return total;
}
