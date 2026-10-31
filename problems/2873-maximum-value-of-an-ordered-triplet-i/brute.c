/* Slow and obviously right: try every triple i < j < k. */
long long maximumTripletValue(int *nums, int numsSize)
{
    long long best = 0, value;
    int i, j, k;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            for (k = j + 1; k < numsSize; k++) {
                value = (long long)(nums[i] - nums[j]) * nums[k];
                if (value > best)
                    best = value;
            }
    return best;
}
