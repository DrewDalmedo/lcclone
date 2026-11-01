/* Reference solution: one pass, keeping the length of the rising and of
 * the falling segment that end at the current element. A step up extends
 * the rising one and restarts the falling one at 1 (the current element
 * alone), a step down does the reverse, and an equal step restarts both. */
int longestMonotonicSubarray(int *nums, int numsSize)
{
    int up = 1, down = 1, best = 1, i;

    for (i = 1; i < numsSize; i++) {
        if (nums[i] > nums[i - 1]) {
            up++;
            down = 1;
        } else if (nums[i] < nums[i - 1]) {
            down++;
            up = 1;
        } else {
            up = 1;
            down = 1;
        }
        if (up > best)
            best = up;
        if (down > best)
            best = down;
    }
    return best;
}
