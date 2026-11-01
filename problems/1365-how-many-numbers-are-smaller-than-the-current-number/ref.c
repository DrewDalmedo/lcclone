/* Reference solution: count how often each value 0..100 occurs, turn the
 * counts into running totals of the values below each one, and look up
 * every element's total. */
int *smallerNumbersThanCurrent(int *nums, int numsSize, int *returnSize)
{
    int below[102] = {0}, *out = malloc((size_t)numsSize * sizeof *out);
    int v, i;

    for (i = 0; i < numsSize; i++)
        below[nums[i] + 1]++;
    for (v = 1; v <= 101; v++)
        below[v] += below[v - 1];
    for (i = 0; i < numsSize; i++)
        out[i] = below[nums[i]];
    *returnSize = numsSize;
    return out;
}
