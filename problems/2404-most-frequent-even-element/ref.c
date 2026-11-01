/* Reference solution: count each even value in a table indexed by half
 * the value, then scan it from the smallest value up, keeping the first
 * value with the highest count. */
int mostFrequentEven(int *nums, int numsSize)
{
    int *count = calloc(50001, sizeof *count);
    int best = -1, bestCount = 0, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] % 2 == 0)
            count[nums[i] / 2]++;
    for (i = 0; i <= 50000; i++)
        if (count[i] > bestCount) {
            bestCount = count[i];
            best = 2 * i;
        }
    free(count);
    return best;
}
