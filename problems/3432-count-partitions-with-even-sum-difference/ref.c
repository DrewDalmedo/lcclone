/* Reference solution: for every coupling, front + back is the whole
 * total, and front - back has the same parity as front + back. So all
 * n - 1 couplings are fair when the total is even, and none when it is
 * odd. */
int countPartitions(int *nums, int numsSize)
{
    int total = 0, i;

    for (i = 0; i < numsSize; i++)
        total += nums[i];
    return total % 2 == 0 ? numsSize - 1 : 0;
}
