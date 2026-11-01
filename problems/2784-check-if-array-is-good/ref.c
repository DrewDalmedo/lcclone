/* Reference solution: k must be nums.length - 1. Count the values in a
 * table, rejecting any value above k before it is used as an index, then
 * check that 1 .. k-1 were seen once each and k twice. */
bool isGood(int *nums, int numsSize)
{
    int count[101] = {0};
    int k = numsSize - 1, i;

    if (k < 1)
        return false;
    for (i = 0; i < numsSize; i++) {
        if (nums[i] > k)
            return false;
        count[nums[i]]++;
    }
    for (i = 1; i < k; i++)
        if (count[i] != 1)
            return false;
    return count[k] == 2;
}
