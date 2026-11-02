/* Reference solution: walk the positions in order, work out each
 * element's digit sum by repeated division by 10, and stop at the first
 * one equal to its position. */
int smallestIndex(int *nums, int numsSize)
{
    int i, v, sum;

    for (i = 0; i < numsSize; i++) {
        sum = 0;
        for (v = nums[i]; v > 0; v /= 10)
            sum += v % 10;
        if (sum == i)
            return i;
    }
    return -1;
}
