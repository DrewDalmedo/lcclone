/* Slow and obviously right, for small cases only: try every selection of
 * entries, by deciding for each entry in turn whether to take it, keep the
 * selections of at most k entries whose values all differ, and remember
 * the one with the largest sum (a long long: three values near 10^9
 * already overflow an int). Then sort its values from largest to
 * smallest. */
static int cur[100], best[100], best_count;
static long long best_sum;

static void try_from(const int *nums, int n, int k, int i, int count,
                     long long sum)
{
    int j, dup = 0;

    if (i == n) {
        if (sum > best_sum) {
            best_sum = sum;
            best_count = count;
            for (j = 0; j < count; j++)
                best[j] = cur[j];
        }
        return;
    }
    try_from(nums, n, k, i + 1, count, sum);
    if (count == k)
        return;
    for (j = 0; j < count; j++)
        if (cur[j] == nums[i])
            dup = 1;
    if (dup)
        return;
    cur[count] = nums[i];
    try_from(nums, n, k, i + 1, count + 1, sum + nums[i]);
}

int *maxKDistinct(int *nums, int numsSize, int k, int *returnSize)
{
    int *out = malloc((size_t)k * sizeof *out);
    int i, j, t;

    best_sum = -1;
    best_count = 0;
    try_from(nums, numsSize, k, 0, 0, 0);
    for (i = 0; i < best_count; i++)
        out[i] = best[i];
    for (i = 1; i < best_count; i++)
        for (j = i; j > 0 && out[j - 1] < out[j]; j--) {
            t = out[j];
            out[j] = out[j - 1];
            out[j - 1] = t;
        }
    *returnSize = best_count;
    return out;
}
