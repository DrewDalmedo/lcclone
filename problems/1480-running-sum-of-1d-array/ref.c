/* Reference solution: carry the running sum along and record it after
 * each move. With at most 1000 moves of at most 10^6 each, every sum lies
 * within -10^9 to 10^9, so an int holds it. */
int *runningSum(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int sum = 0, i;

    for (i = 0; i < numsSize; i++) {
        sum += nums[i];
        out[i] = sum;
    }
    *returnSize = numsSize;
    return out;
}
