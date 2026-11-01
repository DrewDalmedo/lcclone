/* Slow and obviously right: for each k from 1 to n, search nums for k. */
int *findDisappearedNumbers(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int n = 0, k, i, found;

    for (k = 1; k <= numsSize; k++) {
        found = 0;
        for (i = 0; i < numsSize; i++)
            if (nums[i] == k)
                found = 1;
        if (!found)
            out[n++] = k;
    }
    *returnSize = n;
    return out;
}
