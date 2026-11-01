/* Reference solution: one pass that keeps, for each top digit 1 to 9, the
 * largest and second-largest value seen with it (0 for none yet, since
 * every value is at least 1). The answer is the best sum of a top digit's
 * two values, over the top digits that have two. */
int maxSum(int *nums, int numsSize)
{
    int first[10] = {0}, second[10] = {0};
    int best = -1, i, d, x, top;

    for (i = 0; i < numsSize; i++) {
        top = 0;
        for (x = nums[i]; x > 0; x /= 10)
            if (x % 10 > top)
                top = x % 10;
        if (nums[i] > first[top]) {
            second[top] = first[top];
            first[top] = nums[i];
        } else if (nums[i] > second[top]) {
            second[top] = nums[i];
        }
    }
    for (d = 1; d <= 9; d++)
        if (second[d] > 0 && first[d] + second[d] > best)
            best = first[d] + second[d];
    return best;
}
