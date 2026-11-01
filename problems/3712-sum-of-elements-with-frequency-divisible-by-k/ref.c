/* Reference solution: count each value in a table indexed by value (1 to
 * 100), then add value * count for every value that occurs a multiple of
 * k times. */
int sumDivisibleByK(int *nums, int numsSize, int k)
{
    int count[101] = {0}, total = 0, v, i;

    for (i = 0; i < numsSize; i++)
        count[nums[i]]++;
    for (v = 1; v <= 100; v++)
        if (count[v] > 0 && count[v] % k == 0)
            total += v * count[v];
    return total;
}
