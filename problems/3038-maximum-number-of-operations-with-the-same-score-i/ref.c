/* Reference solution: the first two elements fix the score. Walk the pairs
 * (0,1), (2,3), ... and count them until one adds up to something else or
 * fewer than two elements are left. */
int maxOperations(int *nums, int numsSize)
{
    int score = nums[0] + nums[1], count = 0, i;

    for (i = 0; i + 1 < numsSize && nums[i] + nums[i + 1] == score; i += 2)
        count++;
    return count;
}
