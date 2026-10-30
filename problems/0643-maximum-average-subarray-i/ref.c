/* Reference solution: slide a window of k elements along nums, keeping
 * its sum, and divide the largest sum by k at the end. */
double findMaxAverage(int *nums, int numsSize, int k)
{
    long long sum = 0, best;
    int i;

    for (i = 0; i < k; i++)
        sum += nums[i];
    best = sum;
    for (i = k; i < numsSize; i++) {
        sum += nums[i] - nums[i - k];
        if (sum > best)
            best = sum;
    }
    return (double)best / k;
}
