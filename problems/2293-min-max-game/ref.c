/* Reference solution: squeeze in place. Pair j of the current array is
 * read from positions 2j and 2j + 1 before position j is written, since
 * j <= 2j, so the first half of nums can hold the next array. */
int minMaxGame(int *nums, int numsSize)
{
    int m, j, a, b;

    for (m = numsSize; m > 1; m /= 2)
        for (j = 0; j < m / 2; j++) {
            a = nums[2 * j];
            b = nums[2 * j + 1];
            if (j % 2 == 0)
                nums[j] = a < b ? a : b;
            else
                nums[j] = a > b ? a : b;
        }
    return nums[0];
}
