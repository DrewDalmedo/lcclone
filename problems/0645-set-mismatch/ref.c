/* Reference solution: count how often each integer from 1 to n occurs, in
 * a table indexed by the integer, then read the table: the integer counted
 * twice is the doubled one, the one counted zero times the absent one. */
int *findErrorNums(int *nums, int numsSize, int *returnSize)
{
    int *count = calloc((size_t)numsSize + 1, sizeof *count);
    int *out = malloc(2 * sizeof *out), i;

    for (i = 0; i < numsSize; i++)
        count[nums[i]]++;
    for (i = 1; i <= numsSize; i++) {
        if (count[i] == 2)
            out[0] = i;
        else if (count[i] == 0)
            out[1] = i;
    }
    free(count);
    *returnSize = 2;
    return out;
}
