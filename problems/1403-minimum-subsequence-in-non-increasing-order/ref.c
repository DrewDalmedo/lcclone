/* Reference solution: count the values (1 to 100), then keep items from
 * the largest value down until the kept part is worth more than half of
 * the total. The k largest items are worth at least as much as any other
 * k items, so the first k at which they win is the fewest possible, and
 * no other choice of k items is worth as much. */
int *minSubsequence(int *nums, int numsSize, int *returnSize)
{
    int count[101] = {0};
    int total = 0, kept = 0, n = 0, i, v;
    int *out = malloc((size_t)numsSize * sizeof *out);

    for (i = 0; i < numsSize; i++) {
        count[nums[i]]++;
        total += nums[i];
    }
    for (v = 100; v >= 1 && 2 * kept <= total; v--)
        while (count[v] > 0 && 2 * kept <= total) {
            out[n++] = v;
            kept += v;
            count[v]--;
        }
    *returnSize = n;
    return out;
}
