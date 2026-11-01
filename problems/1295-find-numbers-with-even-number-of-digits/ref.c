/* Reference solution: count each number's digits by dividing by 10 until
 * nothing is left, and count the even results. */
int findNumbers(int *nums, int numsSize)
{
    int count = 0, digits, x, i;

    for (i = 0; i < numsSize; i++) {
        digits = 0;
        for (x = nums[i]; x > 0; x /= 10)
            digits++;
        if (digits % 2 == 0)
            count++;
    }
    return count;
}
