/* Reference solution: keep the ant's position, the running sum of the
 * hops, and count the hops after which it is 0. */
int returnToBoundaryCount(int *nums, int numsSize)
{
    int pos = 0, count = 0, i;

    for (i = 0; i < numsSize; i++) {
        pos += nums[i];
        if (pos == 0)
            count++;
    }
    return count;
}
