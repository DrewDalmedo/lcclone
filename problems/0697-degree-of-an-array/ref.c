/* Reference solution: one pass that keeps, for each value, how often it
 * has occurred so far and where it first occurred. When a value's count
 * passes the running degree, its stretch from its first occurrence to
 * here becomes the answer; when the count equals the degree, the shorter
 * of the two stretches is kept. A value's stretch is complete at its last
 * occurrence, which is when its count is final. */
int findShortestSubArray(int *nums, int numsSize)
{
    int *count = calloc(50000, sizeof *count);
    int *first = malloc(50000 * sizeof *first);
    int degree = 0, best = 0, i, v, len;

    for (i = 0; i < numsSize; i++) {
        v = nums[i];
        if (count[v]++ == 0)
            first[v] = i;
        len = i - first[v] + 1;
        if (count[v] > degree) {
            degree = count[v];
            best = len;
        } else if (count[v] == degree && len < best) {
            best = len;
        }
    }
    free(count);
    free(first);
    return best;
}
