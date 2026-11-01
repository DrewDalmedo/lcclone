/* Reference solution: from every start, extend the window one element at
 * a time with a running total, and once its length reaches l (up to r)
 * keep the smallest total above 0. -1 if none was found. */
int minimumSumSubarray(int *nums, int numsSize, int l, int r)
{
    int best = -1, sum, i, j;

    for (i = 0; i < numsSize; i++) {
        sum = 0;
        for (j = i; j < numsSize && j - i + 1 <= r; j++) {
            sum += nums[j];
            if (j - i + 1 >= l && sum > 0 && (best == -1 || sum < best))
                best = sum;
        }
    }
    return best;
}
