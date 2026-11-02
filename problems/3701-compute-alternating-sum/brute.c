/* Slow and obviously right: add up the elements at even indices and the
 * elements at odd indices separately, each in a pass of its own, and
 * subtract the second total from the first. */
int alternatingSum(int *nums, int numsSize)
{
    int even = 0, odd = 0, i;

    for (i = 0; i < numsSize; i++)
        if (i % 2 == 0)
            even += nums[i];
    for (i = 0; i < numsSize; i++)
        if (i % 2 == 1)
            odd += nums[i];
    return even - odd;
}
