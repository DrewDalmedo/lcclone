/* Reference solution: one pass with a write index. Each non-zero value is
 * copied down to the next free place at the front, in order; whatever is
 * left behind it is then filled with zeros. */
void moveZeroes(int *nums, int numsSize)
{
    int w = 0, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] != 0)
            nums[w++] = nums[i];
    while (w < numsSize)
        nums[w++] = 0;
}
