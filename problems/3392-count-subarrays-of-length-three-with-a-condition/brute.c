/* Slow and obviously right: go through every subarray nums[i..k], keep
 * those of length three, and compare the sum of the outer two with half of
 * the middle, worked out exactly as a double. */
int countSubarrays(int *nums, int numsSize)
{
    int count = 0, i, k;

    for (i = 0; i < numsSize; i++)
        for (k = i; k < numsSize; k++)
            if (k - i + 1 == 3 &&
                (double)(nums[i] + nums[k]) == nums[i + 1] / 2.0)
                count++;
    return count;
}
