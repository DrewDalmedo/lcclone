/* Slow and obviously right (for the few items of the small random
 * cases): try every choice of items, as a bit mask, and keep the valid
 * one with the fewest items, then the largest sum. Sort its values from
 * largest to smallest at the end. */
int *minSubsequence(int *nums, int numsSize, int *returnSize)
{
    int total = 0, best_mask = 0, best_size = numsSize + 1, best_sum = -1;
    int n = 0, mask, size, sum, i, k, t;
    int *out = malloc((size_t)numsSize * sizeof *out);

    for (i = 0; i < numsSize; i++)
        total += nums[i];
    for (mask = 1; mask < (1 << numsSize); mask++) {
        size = 0;
        sum = 0;
        for (i = 0; i < numsSize; i++)
            if (mask & (1 << i)) {
                size++;
                sum += nums[i];
            }
        if (sum > total - sum
            && (size < best_size || (size == best_size && sum > best_sum))) {
            best_mask = mask;
            best_size = size;
            best_sum = sum;
        }
    }
    for (i = 0; i < numsSize; i++)
        if (best_mask & (1 << i))
            out[n++] = nums[i];
    for (i = 1; i < n; i++)
        for (k = i; k > 0 && out[k - 1] < out[k]; k--) {
            t = out[k];
            out[k] = out[k - 1];
            out[k - 1] = t;
        }
    *returnSize = n;
    return out;
}
