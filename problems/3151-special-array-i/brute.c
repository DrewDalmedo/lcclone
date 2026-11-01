/* Slow and obviously right: an alternating array follows one of two fixed
 * patterns, odd-even-odd-... or even-odd-even-...; try both. */
static bool follows(const int *nums, int n, int first_odd)
{
    int i;

    for (i = 0; i < n; i++)
        if (nums[i] % 2 != (i + first_odd) % 2)
            return false;
    return true;
}

bool isArraySpecial(int *nums, int numsSize)
{
    return follows(nums, numsSize, 0) || follows(nums, numsSize, 1);
}
