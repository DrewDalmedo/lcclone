/* Reference solution: one pass. Work out each entry's digit range; a
 * larger range than any so far restarts the sum at that entry, and an
 * equal one adds the entry to the sum. */
static int digit_range(int v)
{
    int hi = 0, lo = 9, d;

    while (v > 0) {
        d = v % 10;
        if (d > hi)
            hi = d;
        if (d < lo)
            lo = d;
        v /= 10;
    }
    return hi - lo;
}

int maxDigitRange(int *nums, int numsSize)
{
    int best = -1, sum = 0, i, r;

    for (i = 0; i < numsSize; i++) {
        r = digit_range(nums[i]);
        if (r > best) {
            best = r;
            sum = nums[i];
        } else if (r == best) {
            sum += nums[i];
        }
    }
    return sum;
}
