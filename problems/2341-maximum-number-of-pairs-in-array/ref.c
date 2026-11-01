/* Reference solution: count each number in a table of 101 slots. A number
 * on c marbles gives c / 2 pairs, and the marbles not drawn are the rest. */
int *numberOfPairs(int *nums, int numsSize, int *returnSize)
{
    int count[101] = {0}, pairs = 0, v, i;
    int *out = malloc(2 * sizeof *out);

    for (i = 0; i < numsSize; i++)
        count[nums[i]]++;
    for (v = 0; v <= 100; v++)
        pairs += count[v] / 2;
    out[0] = pairs;
    out[1] = numsSize - 2 * pairs;
    *returnSize = 2;
    return out;
}
