/* Slow and obviously right: for every pile, add up the piles before it
 * and the piles after it from scratch, and take the difference. */
int *leftRightDifference(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    long long left, right;
    int i, k;

    for (i = 0; i < numsSize; i++) {
        left = 0;
        right = 0;
        for (k = 0; k < numsSize; k++) {
            if (k < i)
                left += nums[k];
            else if (k > i)
                right += nums[k];
        }
        out[i] = (int)(left > right ? left - right : right - left);
    }
    *returnSize = numsSize;
    return out;
}
