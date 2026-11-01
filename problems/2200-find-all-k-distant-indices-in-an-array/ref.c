/* Reference solution: go through the lamps (positions j with nums[j] ==
 * key) from left to right. Each lights [j - k, j + k], cut to the array;
 * start writing that window after the last index already written, so the
 * result stays increasing and holds each index once. O(n). */
int *findKDistantIndices(int *nums, int numsSize, int key, int k,
                         int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int m = 0, next = 0, i, j, hi;

    for (j = 0; j < numsSize; j++)
        if (nums[j] == key) {
            i = j - k > next ? j - k : next;
            hi = j + k < numsSize - 1 ? j + k : numsSize - 1;
            for (; i <= hi; i++)
                out[m++] = i;
            if (hi + 1 > next)
                next = hi + 1;
        }
    *returnSize = m;
    return out;
}
