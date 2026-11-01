/* Slow and obviously right: try every x from 0 to 1001 (one more than the
 * largest value allowed) and count the elements >= x from scratch. */
int specialArray(int *nums, int numsSize)
{
    int x, i, count;

    for (x = 0; x <= 1001; x++) {
        count = 0;
        for (i = 0; i < numsSize; i++)
            if (nums[i] >= x)
                count++;
        if (count == x)
            return x;
    }
    return -1;
}
