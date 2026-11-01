/* Reference solution: the largest remaining square is always at one end
 * of nums, so walk two indices in from the ends and fill the answer from
 * the back, taking the end with the larger absolute value each time. */
int *sortedSquares(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int lo = 0, hi = numsSize - 1, pos = numsSize - 1;

    while (lo <= hi) {
        int a = nums[lo] * nums[lo], b = nums[hi] * nums[hi];

        if (a > b) {
            out[pos--] = a;
            lo++;
        } else {
            out[pos--] = b;
            hi--;
        }
    }
    *returnSize = numsSize;
    return out;
}
