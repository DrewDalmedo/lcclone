/* Reference solution: cut nums into maximal stretches of equal neighbours
 * and count, for each value, how many stretches it starts. A value
 * appears in a single block exactly when it starts one stretch. */
int countSpecialIntegers(int *nums, int numsSize)
{
    int blocks[101] = {0};
    int i, v, count = 0;

    for (i = 0; i < numsSize; i++)
        if (i == 0 || nums[i] != nums[i - 1])
            blocks[nums[i]]++;
    for (v = 1; v <= 100; v++)
        if (blocks[v] == 1)
            count++;
    return count;
}
