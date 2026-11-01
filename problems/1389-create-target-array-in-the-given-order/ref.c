/* Reference solution: keep the row in an array of n slots. For each step,
 * move the numbers from position index[i] on one place to the right with
 * memmove (which copies overlapping memory safely), then write nums[i]
 * into the gap. */
int *createTargetArray(int *nums, int numsSize, int *index, int indexSize,
                       int *returnSize)
{
    int *row = malloc((size_t)numsSize * sizeof *row);
    int i, p;

    (void)indexSize;
    for (i = 0; i < numsSize; i++) {
        p = index[i];
        if (p < i)
            memmove(row + p + 1, row + p, (size_t)(i - p) * sizeof *row);
        row[p] = nums[i];
    }
    *returnSize = numsSize;
    return row;
}
