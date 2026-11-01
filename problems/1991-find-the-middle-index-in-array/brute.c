/* Slow and obviously right: for each index from the left, add up the
 * elements in front of it and the elements behind it from scratch, and
 * return the first index where the two sums agree. */
int findMiddleIndex(int *nums, int numsSize)
{
    int i, j, before, after;

    for (i = 0; i < numsSize; i++) {
        before = 0;
        after = 0;
        for (j = 0; j < i; j++)
            before += nums[j];
        for (j = i + 1; j < numsSize; j++)
            after += nums[j];
        if (before == after)
            return i;
    }
    return -1;
}
