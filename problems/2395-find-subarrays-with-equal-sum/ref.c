/* Reference solution: put the n - 1 neighbour sums in an array, sort it,
 * and look for two equal sums side by side. Each sum lies within
 * +-2 * 10^9, so it fits in an int, but the difference of two sums may
 * not: the comparison avoids subtracting. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

bool findSubarrays(int *nums, int numsSize)
{
    int m = numsSize - 1, i;
    int *sums = malloc((size_t)m * sizeof *sums);
    bool found = false;

    for (i = 0; i < m; i++)
        sums[i] = nums[i] + nums[i + 1];
    qsort(sums, (size_t)m, sizeof *sums, cmp_int);
    for (i = 1; i < m && !found; i++)
        found = sums[i] == sums[i - 1];
    free(sums);
    return found;
}
