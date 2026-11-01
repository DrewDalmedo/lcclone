/* Slow and obviously right: for each entry, count the entries equal to
 * it; then count the entries whose count is the largest of them all. */
int maxFrequencyElements(int *nums, int numsSize)
{
    int *same = malloc((size_t)numsSize * sizeof *same);
    int top = 0, total = 0, i, k;

    for (i = 0; i < numsSize; i++) {
        same[i] = 0;
        for (k = 0; k < numsSize; k++)
            if (nums[k] == nums[i])
                same[i]++;
        if (same[i] > top)
            top = same[i];
    }
    for (i = 0; i < numsSize; i++)
        if (same[i] == top)
            total++;
    free(same);
    return total;
}
