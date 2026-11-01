/* Slow and obviously right: try 0, 1, ..., n in turn, and return the
 * first one that no element equals. */
int missingNumber(int *nums, int numsSize)
{
    int v, i;

    for (v = 0; v <= numsSize; v++) {
        for (i = 0; i < numsSize && nums[i] != v; i++)
            ;
        if (i == numsSize)
            return v;
    }
    return -1;
}
