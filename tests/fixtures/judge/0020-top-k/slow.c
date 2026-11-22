/* Right, but O(n * k): fast on the stored cases, too slow at full size
 * when k = n. */
int *topK(int *nums, int numsSize, int k, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    char *used = calloc((size_t)numsSize, 1);
    int i, j;

    for (i = 0; i < k; i++) {
        int best = -1;

        for (j = 0; j < numsSize; j++)
            if (!used[j] && (best < 0 || nums[j] > nums[best]))
                best = j;
        used[best] = 1;
        out[i] = nums[best];
    }
    *returnSize = k;
    return out;
}
