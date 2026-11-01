/* Reference solution: with v to start, the balance after transaction k is
 * v plus the k-th prefix sum, so only the lowest prefix sum matters. Track
 * it from 0 (which builds in the rule that v is at least 1) and return
 * 1 minus it. */
int minStartValue(int *nums, int numsSize)
{
    int sum = 0, low = 0, i;

    for (i = 0; i < numsSize; i++) {
        sum += nums[i];
        if (sum < low)
            low = sum;
    }
    return 1 - low;
}
