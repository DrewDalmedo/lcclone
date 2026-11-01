/* Slow and obviously right: record only which row each number went to.
 * A row's rightmost number is found by searching back for the latest
 * number given to that row, and the answer lists the numbers of row 0,
 * then those of row 1, each in the original order. */
int *resultArray(int *nums, int numsSize, int *returnSize)
{
    int row[50];
    int *out = malloc((size_t)numsSize * sizeof *out);
    int last[2] = {0, 0};
    int i, j, k, r, m = 0;

    for (i = 0; i < numsSize; i++) {
        if (i < 2) {
            row[i] = i;
            continue;
        }
        for (r = 0; r < 2; r++)
            for (j = i - 1; j >= 0; j--)
                if (row[j] == r) {
                    last[r] = nums[j];
                    break;
                }
        row[i] = last[0] > last[1] ? 0 : 1;
    }
    for (r = 0; r < 2; r++)
        for (k = 0; k < numsSize; k++)
            if (row[k] == r)
                out[m++] = nums[k];
    *returnSize = m;
    return out;
}
