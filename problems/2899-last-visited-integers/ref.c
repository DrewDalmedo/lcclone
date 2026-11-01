/* Reference solution: keep the positive entries in order in an array, and
 * count the lookups of the current streak; the t-th lookup reads the t-th
 * element from the end of the array. */
int *lastVisitedIntegers(int *nums, int numsSize, int *returnSize)
{
    int *seen = malloc((size_t)numsSize * sizeof *seen);
    int *out = malloc((size_t)numsSize * sizeof *out);
    int logged = 0, answers = 0, t = 0, i;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] > 0) {
            seen[logged++] = nums[i];
            t = 0;
        } else {
            t++;
            out[answers++] = t <= logged ? seen[logged - t] : -1;
        }
    }
    free(seen);
    *returnSize = answers;
    return out;
}
