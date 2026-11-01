/* Reference solution: with the total T of nums, after(i) is
 * T - before(i) - nums[i]. One pass from the left carries before(i), so
 * each index is settled in O(1), and the first one that qualifies is the
 * answer. All sums stay within 10^5, so int is enough. */
int findMiddleIndex(int *nums, int numsSize)
{
    int total = 0, before = 0, i;

    for (i = 0; i < numsSize; i++)
        total += nums[i];
    for (i = 0; i < numsSize; i++) {
        if (before == total - before - nums[i])
            return i;
        before += nums[i];
    }
    return -1;
}
