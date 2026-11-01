/* Slow and obviously right: for each cut from the left, scan both pieces
 * afresh for their largest and smallest numbers. */
int firstStableIndex(int *nums, int numsSize, int k)
{
    int i, j, big, small;

    for (i = 0; i < numsSize; i++) {
        big = nums[0];
        for (j = 0; j <= i; j++)
            if (nums[j] > big)
                big = nums[j];
        small = nums[i];
        for (j = i; j < numsSize; j++)
            if (nums[j] < small)
                small = nums[j];
        if ((long long)big - small <= k)
            return i;
    }
    return -1;
}
