/* Reference solution: for each value, remember the last two positions it
 * was seen at. The spread of three positions is twice the gap between the
 * outer two, so the best trio of a value is three of its copies in a row;
 * each new copy closes one such trio with the two copies before it. */
int minimumDistance(int *nums, int numsSize)
{
    int last[101], before[101];
    int best = -1, i, v;

    for (v = 0; v <= numsSize; v++)
        last[v] = before[v] = -1;
    for (i = 0; i < numsSize; i++) {
        v = nums[i];
        if (before[v] >= 0 && (best < 0 || 2 * (i - before[v]) < best))
            best = 2 * (i - before[v]);
        before[v] = last[v];
        last[v] = i;
    }
    return best;
}
