/* Reference solution: one pass. Before index k, keep the largest element
 * seen and the largest difference nums[i] - nums[j] with i < j; the best
 * triple ending at k is that difference times nums[k], formed in 64 bits. */
long long maximumTripletValue(int *nums, int numsSize)
{
    long long best = 0, diff = 0, value;
    int top = nums[0], k;

    for (k = 1; k < numsSize; k++) {
        value = diff * nums[k];
        if (value > best)
            best = value;
        if (top - nums[k] > diff)
            diff = top - nums[k];
        if (nums[k] > top)
            top = nums[k];
    }
    return best;
}
