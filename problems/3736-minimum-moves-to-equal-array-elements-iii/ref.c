/* Reference solution: towers only grow, so the common height is at least
 * the tallest one, and every block beyond that would be wasted. The
 * answer is the sum of max - nums[i], that is n * max - sum (at most
 * 99 * 99, well inside an int). */
int minMoves(int *nums, int numsSize)
{
    int max = nums[0], sum = 0, i;

    for (i = 0; i < numsSize; i++) {
        sum += nums[i];
        if (nums[i] > max)
            max = nums[i];
    }
    return numsSize * max - sum;
}
