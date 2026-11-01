/* Reference solution: add up the short (one-digit) and the long
 * (two-digit) numbers separately. Claiming the larger total wins, so Alice
 * can win unless the two totals are equal. */
bool canAliceWin(int *nums, int numsSize)
{
    int shortSum = 0, longSum = 0, i;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] < 10)
            shortSum += nums[i];
        else
            longSum += nums[i];
    }
    return shortSum != longSum;
}
