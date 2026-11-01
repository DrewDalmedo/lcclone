/* Reference solution: one pass that merges and packs at once. A merge at i
 * only zeroes position i + 1 (and the next step then sees that 0, which
 * merges with nothing that matters), so: at a non-zero value equal to its
 * right neighbour, write its double and skip the neighbour; at any other
 * non-zero value, write it; skip zeros. Fill the rest with zeros. */
int *applyOperations(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int i = 0, k = 0;

    while (i < numsSize) {
        if (nums[i] == 0) {
            i++;
        } else if (i + 1 < numsSize && nums[i + 1] == nums[i]) {
            out[k++] = 2 * nums[i];
            i += 2;
        } else {
            out[k++] = nums[i];
            i++;
        }
    }
    while (k < numsSize)
        out[k++] = 0;
    *returnSize = numsSize;
    return out;
}
