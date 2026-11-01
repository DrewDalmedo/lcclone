/* Reference solution: each move lowers the total by exactly 1, whichever
 * pile it takes from, so the fewest moves is the total's remainder when
 * divided by k. The total is at most 10^6, so an int holds it. */
int minOperations(int *nums, int numsSize, int k)
{
    int total = 0, i;

    for (i = 0; i < numsSize; i++)
        total += nums[i];
    return total % k;
}
