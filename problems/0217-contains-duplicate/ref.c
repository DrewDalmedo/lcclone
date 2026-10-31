/* Reference solution: add the values one by one to a hash set (open
 * addressing with linear probing, sized to at least twice the count);
 * finding a value already in the set means it repeats. Expected O(n). */
bool containsDuplicate(int *nums, int numsSize)
{
    int bits = 1, i;
    size_t cap, mask, h;
    int *keys;
    unsigned char *used;
    bool found = false;

    while (((size_t)1 << bits) < 2 * (size_t)numsSize)
        bits++;
    cap = (size_t)1 << bits;
    mask = cap - 1;
    keys = malloc(cap * sizeof *keys);
    used = calloc(cap, 1);
    for (i = 0; i < numsSize && !found; i++) {
        /* Fibonacci hashing of the value's 32 bits: the top bits of the
         * product, so negative values hash as well as positive ones. */
        h = (size_t)(((uint64_t)(uint32_t)nums[i] * 0x9E3779B97F4A7C15ull)
                     >> (64 - bits));
        while (used[h] && keys[h] != nums[i])
            h = (h + 1) & mask;
        if (used[h]) {
            found = true;
        } else {
            used[h] = 1;
            keys[h] = nums[i];
        }
    }
    free(keys);
    free(used);
    return found;
}
