/* Slow and obviously right: add up every window of k elements anew. */
double findMaxAverage(int *nums, int numsSize, int k)
{
    long long best = 0, sum;
    int start, i;

    for (start = 0; start + k <= numsSize; start++) {
        sum = 0;
        for (i = start; i < start + k; i++)
            sum += nums[i];
        if (start == 0 || sum > best)
            best = sum;
    }
    return (double)best / k;
}
