/* Reference solution: one pass from the right marks the posts taller than
 * everything after them; a pass from the left then keeps, in index order,
 * each post marked or taller than everything before it. */
int *findValidElements(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    char *right = malloc((size_t)numsSize);
    int best = 0, m = 0, i;

    for (i = numsSize - 1; i >= 0; i--) {
        right[i] = nums[i] > best;
        if (nums[i] > best)
            best = nums[i];
    }
    best = 0;
    for (i = 0; i < numsSize; i++) {
        if (nums[i] > best || right[i])
            out[m++] = nums[i];
        if (nums[i] > best)
            best = nums[i];
    }
    free(right);
    *returnSize = m;
    return out;
}
