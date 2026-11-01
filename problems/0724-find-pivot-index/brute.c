/* Slow and obviously right: for each index from the left, add up the
 * elements before it and the elements after it anew, and compare. */
int pivotIndex(int *nums, int numsSize)
{
    int i, j, left, right;

    for (i = 0; i < numsSize; i++) {
        left = 0;
        right = 0;
        for (j = 0; j < i; j++)
            left += nums[j];
        for (j = i + 1; j < numsSize; j++)
            right += nums[j];
        if (left == right)
            return i;
    }
    return -1;
}
