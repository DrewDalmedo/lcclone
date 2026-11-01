/* Slow and obviously right: try every value L from min - k to max + k as
 * the lowest point of the shifted array. Each element v can reach the
 * values v - k to v + k; if one can't reach L or above, L is impossible.
 * Otherwise each element goes as low as it can without going under L,
 * which is max(L, v - k), and the spread is the highest of those minus L.
 * Return the smallest spread over all L. */
int smallestRangeI(int *nums, int numsSize, int k)
{
    int lo = nums[0], hi = nums[0], best = -1, L, i, low, top, ok;

    for (i = 1; i < numsSize; i++) {
        if (nums[i] < lo)
            lo = nums[i];
        if (nums[i] > hi)
            hi = nums[i];
    }
    for (L = lo - k; L <= hi + k; L++) {
        ok = 1;
        top = L;
        for (i = 0; i < numsSize; i++) {
            if (nums[i] + k < L) {
                ok = 0;
                break;
            }
            low = nums[i] - k > L ? nums[i] - k : L;
            if (low > top)
                top = low;
        }
        if (ok && (best < 0 || top - L < best))
            best = top - L;
    }
    return best;
}
