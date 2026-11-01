/* Reference solution: every value but one occurs once, so the first value
 * met for the second time is the answer. A table of 10^4 + 1 flags
 * remembers the values seen. */
int repeatedNTimes(int *nums, int numsSize)
{
    bool seen[10001] = {false};
    int i;

    for (i = 0; i < numsSize; i++) {
        if (seen[nums[i]])
            return nums[i];
        seen[nums[i]] = true;
    }
    return -1; /* not reached: some value repeats */
}
