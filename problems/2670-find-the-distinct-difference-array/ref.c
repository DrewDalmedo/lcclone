/* Reference solution: one pass from the right records, for every i, how
 * many different values nums[i..n-1] holds; one pass from the left counts
 * the different values seen so far and subtracts the count for the part
 * after i. Values are at most 50, so a table of 51 flags marks them. */
int *distinctDifferenceArray(int *nums, int numsSize, int *returnSize)
{
    int *answer = malloc((size_t)numsSize * sizeof *answer);
    int *after = malloc(((size_t)numsSize + 1) * sizeof *after);
    char seen[51];
    int i, count;

    memset(seen, 0, sizeof seen);
    after[numsSize] = 0;
    count = 0;
    for (i = numsSize - 1; i >= 0; i--) {
        if (!seen[nums[i]]) {
            seen[nums[i]] = 1;
            count++;
        }
        after[i] = count;
    }
    memset(seen, 0, sizeof seen);
    count = 0;
    for (i = 0; i < numsSize; i++) {
        if (!seen[nums[i]]) {
            seen[nums[i]] = 1;
            count++;
        }
        answer[i] = count - after[i + 1];
    }
    free(after);
    *returnSize = numsSize;
    return answer;
}
