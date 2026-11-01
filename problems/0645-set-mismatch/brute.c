/* Slow and obviously right: for each integer from 1 to n, go through the
 * whole of nums counting where it occurs. The one found twice is the
 * doubled one, the one found nowhere the absent one. */
int *findErrorNums(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc(2 * sizeof *out), v, i, count;

    for (v = 1; v <= numsSize; v++) {
        count = 0;
        for (i = 0; i < numsSize; i++)
            if (nums[i] == v)
                count++;
        if (count == 2)
            out[0] = v;
        if (count == 0)
            out[1] = v;
    }
    *returnSize = 2;
    return out;
}
