/* Reference solution: a number x adds x to the true total and the sum of
 * its digits to the machine's, so add up x minus its digit sum over all
 * of nums. The answer is at most 2000 * 1998, well within an int. */
int differenceOfSum(int *nums, int numsSize)
{
    int total = 0, digits, x, i;

    for (i = 0; i < numsSize; i++) {
        digits = 0;
        for (x = nums[i]; x > 0; x /= 10)
            digits += x % 10;
        total += nums[i] - digits;
    }
    return total;
}
