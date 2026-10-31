/* Reference solution: one pass with a hash map (open addressing, linear
 * probing) from each value to the last position it was seen at. A value
 * seen again at most k positions after its last occurrence is a near
 * repeat; otherwise its last position moves up. Expected O(n). */
bool containsNearbyDuplicate(int *nums, int numsSize, int k)
{
    int bits = 1, i;
    size_t cap, mask, h;
    int *keys, *last;
    unsigned char *used;
    bool found = false;

    while (((size_t)1 << bits) < 2 * (size_t)numsSize)
        bits++;
    cap = (size_t)1 << bits;
    mask = cap - 1;
    keys = malloc(cap * sizeof *keys);
    last = malloc(cap * sizeof *last);
    used = calloc(cap, 1);
    for (i = 0; i < numsSize && !found; i++) {
        /* Fibonacci hashing of the value's 32 bits (the top bits of the
         * product), so negative values hash as well as positive ones. */
        h = (size_t)(((uint64_t)(uint32_t)nums[i] * 0x9E3779B97F4A7C15ull)
                     >> (64 - bits));
        while (used[h] && keys[h] != nums[i])
            h = (h + 1) & mask;
        if (used[h] && i - last[h] <= k) {
            found = true;
        } else {
            used[h] = 1;
            keys[h] = nums[i];
            last[h] = i;
        }
    }
    free(keys);
    free(last);
    free(used);
    return found;
}
