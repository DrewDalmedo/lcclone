/* Slow and obviously right: all that matters is which elements end up
 * flipped an odd number of times. Try every set S of elements to end up
 * negated: k flips can do it when |S| <= k and k - |S| is even, since the
 * extra flips then go in pairs to any element. Exponential in the length
 * of nums, so only for the small random cases. */
int largestSumAfterKNegations(int *nums, int numsSize, int k)
{
    int best = 0, found = 0, size, sum, i;
    unsigned mask;

    for (mask = 0; mask < (1u << numsSize); mask++) {
        size = 0;
        sum = 0;
        for (i = 0; i < numsSize; i++) {
            if (mask >> i & 1u) {
                size++;
                sum -= nums[i];
            } else {
                sum += nums[i];
            }
        }
        if (size <= k && (k - size) % 2 == 0 && (!found || sum > best)) {
            best = sum;
            found = 1;
        }
    }
    return best;
}
