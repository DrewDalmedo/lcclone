/* Reference solution: count each value in a table offset by 100, then
 * write the values out bucket by bucket: count 1 first, then count 2, and
 * so on, and within one count from the largest value down. */
int *frequencySort(int *nums, int numsSize, int *returnSize)
{
    int cnt[201] = {0};
    int *out = malloc((size_t)numsSize * sizeof *out);
    int i, c, v, len = 0;

    for (i = 0; i < numsSize; i++)
        cnt[nums[i] + 100]++;
    for (c = 1; c <= numsSize && len < numsSize; c++)
        for (v = 100; v >= -100; v--)
            if (cnt[v + 100] == c)
                for (i = 0; i < c; i++)
                    out[len++] = v;
    *returnSize = numsSize;
    return out;
}
