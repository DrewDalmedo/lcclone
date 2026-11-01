/* Reference solution: scan from the front and return the first index i
 * with nums[i] == i % 10. */
int smallestEqual(int *nums, int numsSize)
{
    int i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] == i % 10)
            return i;
    return -1;
}
