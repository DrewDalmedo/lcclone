/* Reference solution: count each value in a table indexed by value. When
 * any pair works, x is the smallest value m of nums (m is smaller than
 * both members of any other working pair, and its count differs
 * from at least one of theirs). So y is the first value above m that
 * occurs and has a count other than m's. */
int *minDistinctFreqPair(int *nums, int numsSize, int *returnSize)
{
    int count[101] = {0};
    int *out = malloc(2 * sizeof *out);
    int m = 101, i, v;

    for (i = 0; i < numsSize; i++) {
        count[nums[i]]++;
        if (nums[i] < m)
            m = nums[i];
    }
    out[0] = out[1] = -1;
    for (v = m + 1; v <= 100; v++)
        if (count[v] > 0 && count[v] != count[m]) {
            out[0] = m;
            out[1] = v;
            break;
        }
    *returnSize = 2;
    return out;
}
