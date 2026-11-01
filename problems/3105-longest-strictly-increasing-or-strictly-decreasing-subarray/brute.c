/* Slow and obviously right: test every segment from the definition. */
int longestMonotonicSubarray(int *nums, int numsSize)
{
    int best = 0, i, j, t, rising, falling;

    for (i = 0; i < numsSize; i++)
        for (j = i; j < numsSize; j++) {
            rising = falling = 1;
            for (t = i + 1; t <= j; t++) {
                if (nums[t] <= nums[t - 1])
                    rising = 0;
                if (nums[t] >= nums[t - 1])
                    falling = 0;
            }
            if ((rising || falling) && j - i + 1 > best)
                best = j - i + 1;
        }
    return best;
}
