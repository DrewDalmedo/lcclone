/* Slow and obviously right: for each even entry, from the front, look at
 * every other entry to see whether any holds the same number. */
int firstUniqueEven(int *nums, int numsSize)
{
    int i, j, alone;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] % 2 != 0)
            continue;
        alone = 1;
        for (j = 0; j < numsSize; j++)
            if (j != i && nums[j] == nums[i])
                alone = 0;
        if (alone)
            return nums[i];
    }
    return -1;
}
