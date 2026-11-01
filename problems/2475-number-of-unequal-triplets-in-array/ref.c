/* Reference solution: count each value, then walk the values in
 * increasing order. A value seen c times can be the middle value of a
 * good choice together with any of the `left` smaller elements and any of
 * the larger ones, so it adds left * c * (n - left - c). */
int unequalTriplets(int *nums, int numsSize)
{
    int count[1001] = {0};
    int left = 0, total = 0, v, i;

    for (i = 0; i < numsSize; i++)
        count[nums[i]]++;
    for (v = 1; v <= 1000; v++) {
        if (count[v] == 0)
            continue;
        total += left * count[v] * (numsSize - left - count[v]);
        left += count[v];
    }
    return total;
}
