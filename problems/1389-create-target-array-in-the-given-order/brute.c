/* Slow and obviously right, by another method: never move the numbers,
 * only track the position of each one placed so far. Slotting a number
 * in at p pushes every number at position p or beyond one place to the
 * right. At the end, number i is written at its final position. */
int *createTargetArray(int *nums, int numsSize, int *index, int indexSize,
                       int *returnSize)
{
    int pos[100];
    int *row = malloc((size_t)numsSize * sizeof *row);
    int i, j;

    (void)indexSize;
    for (i = 0; i < numsSize; i++) {
        for (j = 0; j < i; j++)
            if (pos[j] >= index[i])
                pos[j]++;
        pos[i] = index[i];
    }
    for (i = 0; i < numsSize; i++)
        row[pos[i]] = nums[i];
    *returnSize = numsSize;
    return row;
}
