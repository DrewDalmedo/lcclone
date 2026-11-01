/* Reference solution: take cards from the end of nums, setting bit v of a
 * 64-bit mask for each number v <= k, and stop as soon as bits 1..k are
 * all set. Numbers go up to 50, so the mask can't be a 32-bit int. */
int minOperations(int *nums, int numsSize, int k)
{
    unsigned long long seen = 0, full = ((1ULL << k) - 1) << 1;
    int i;

    for (i = numsSize - 1; i >= 0; i--) {
        if (nums[i] <= k)
            seen |= 1ULL << nums[i];
        if (seen == full)
            return numsSize - i;
    }
    return numsSize;
}
