/* Slow and obviously right: try every pair of positions i < j, finding
 * each value's top digit through its decimal text, and keep the largest
 * sum of a pair whose top digits match. */
static char top_digit(int x)
{
    char text[16], top = '0';
    int k;

    sprintf(text, "%d", x);
    for (k = 0; text[k]; k++)
        if (text[k] > top)
            top = text[k];
    return top;
}

int maxSum(int *nums, int numsSize)
{
    int best = -1, i, j;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            if (top_digit(nums[i]) == top_digit(nums[j])
                && nums[i] + nums[j] > best)
                best = nums[i] + nums[j];
    return best;
}
