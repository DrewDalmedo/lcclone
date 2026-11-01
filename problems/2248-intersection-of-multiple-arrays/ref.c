/* Reference solution: no list repeats a number, so a number is in every
 * list exactly when it is counted numsSize times. Count each number in a
 * table indexed by its value, then walk the table upwards, which lists the
 * common numbers in increasing order. */
int *intersection(int **nums, int numsSize, int *numsColSize, int *returnSize)
{
    int count[1001] = {0};
    int *out = malloc(1000 * sizeof *out);
    int m = 0, i, j, v;

    for (i = 0; i < numsSize; i++)
        for (j = 0; j < numsColSize[i]; j++)
            count[nums[i][j]]++;
    for (v = 1; v <= 1000; v++)
        if (count[v] == numsSize)
            out[m++] = v;
    *returnSize = m;
    return out;
}
