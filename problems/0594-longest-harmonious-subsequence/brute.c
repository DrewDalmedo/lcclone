/* Slow and obviously right: try each element's value v as the smallest
 * value of the subsequence, and take every element equal to v or v + 1.
 * That is harmonious only when some element is v + 1. */
int findLHS(int *nums, int numsSize)
{
    int best = 0, count, above, i, k;

    for (i = 0; i < numsSize; i++) {
        count = 0;
        above = 0;
        for (k = 0; k < numsSize; k++) {
            if (nums[k] == nums[i]) {
                count++;
            } else if ((long long)nums[k] == (long long)nums[i] + 1) {
                count++;
                above = 1;
            }
        }
        if (above && count > best)
            best = count;
    }
    return best;
}
