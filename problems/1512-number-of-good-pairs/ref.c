/* Reference solution: walk the array once, counting how often each value
 * has been seen so far. Each element pairs with every earlier copy of its
 * value, so add that count before bumping it. */
int numIdenticalPairs(int *nums, int numsSize)
{
    int seen[101] = {0};
    int pairs = 0, i;

    for (i = 0; i < numsSize; i++) {
        pairs += seen[nums[i]];
        seen[nums[i]]++;
    }
    return pairs;
}
