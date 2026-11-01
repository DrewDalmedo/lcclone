/* Slow and obviously right: straight from the definition, test every
 * index against every other one, without looking for the largest first. */
int dominantIndex(int *nums, int numsSize)
{
    int i, j;

    for (i = 0; i < numsSize; i++) {
        for (j = 0; j < numsSize; j++)
            if (j != i && nums[i] < 2 * nums[j])
                break;
        if (j == numsSize)
            return i;
    }
    return -1;
}
