/* Reference solution: find the smallest and the largest value in one
 * pass, then apply Euclid's algorithm to the pair. */
int findGCD(int *nums, int numsSize)
{
    int lo = nums[0], hi = nums[0], i, r;

    for (i = 1; i < numsSize; i++) {
        if (nums[i] < lo)
            lo = nums[i];
        if (nums[i] > hi)
            hi = nums[i];
    }
    while (lo != 0) {
        r = hi % lo;
        hi = lo;
        lo = r;
    }
    return hi;
}
