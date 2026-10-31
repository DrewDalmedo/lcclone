/* Reference solution: an open-addressing hash table from value to index,
 * filled while scanning, so each element is looked at once. */
int *twoSum(int *nums, int numsSize, int target, int *returnSize)
{
    int cap = 1, i, *keys, *idx, *out;

    while (cap < 2 * numsSize)
        cap *= 2;
    keys = malloc((size_t)cap * sizeof *keys);
    idx = malloc((size_t)cap * sizeof *idx);
    for (i = 0; i < cap; i++)
        idx[i] = -1;
    out = malloc(2 * sizeof *out);
    *returnSize = 0;
    for (i = 0; i < numsSize; i++) {
        long long want = (long long)target - nums[i];
        unsigned h = (unsigned)(want * 2654435761u) & (unsigned)(cap - 1);

        if (want >= INT_MIN && want <= INT_MAX) {
            while (idx[h] >= 0 && keys[h] != (int)want)
                h = (h + 1) & (unsigned)(cap - 1);
            if (idx[h] >= 0) {
                out[0] = idx[h];
                out[1] = i;
                *returnSize = 2;
                break;
            }
        }
        h = (unsigned)((long long)nums[i] * 2654435761u) &
            (unsigned)(cap - 1);
        while (idx[h] >= 0 && keys[h] != nums[i])
            h = (h + 1) & (unsigned)(cap - 1);
        if (idx[h] < 0) {
            keys[h] = nums[i];
            idx[h] = i;
        }
    }
    free(keys);
    free(idx);
    return out;
}
