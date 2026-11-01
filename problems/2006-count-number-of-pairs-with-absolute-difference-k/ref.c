/* Reference solution: one pass from left to right, counting how many times
 * each value has been seen so far. Each value pairs with every earlier
 * copy of value - k and of value + k, when those lie in 1..100. */
int countKDifference(int *nums, int numsSize, int k)
{
    int seen[101] = {0};
    int count = 0, i, v;

    for (i = 0; i < numsSize; i++) {
        v = nums[i];
        if (v - k >= 1)
            count += seen[v - k];
        if (v + k <= 100)
            count += seen[v + k];
        seen[v]++;
    }
    return count;
}
