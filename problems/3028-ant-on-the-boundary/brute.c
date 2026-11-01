/* Slow and obviously right: walk the ant one unit at a time, and look at
 * where it stands only once each hop is over. */
int returnToBoundaryCount(int *nums, int numsSize)
{
    int pos = 0, count = 0, i, step;

    for (i = 0; i < numsSize; i++) {
        for (step = 0; step < abs(nums[i]); step++)
            pos += nums[i] > 0 ? 1 : -1;
        if (pos == 0)
            count++;
    }
    return count;
}
