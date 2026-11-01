/* Reference solution: the cheapest way to pick c tasks is to pick the c
 * shortest. Sort a copy of nums, take prefix sums (p[c] = the c smallest
 * added up), and for each query binary-search the largest c with
 * p[c] <= q. The total is at most 10^9, so the sums fit in an int. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int *answerQueries(int *nums, int numsSize, int *queries, int queriesSize,
                   int *returnSize)
{
    int *p = malloc((size_t)(numsSize + 1) * sizeof *p);
    int *out = malloc((size_t)queriesSize * sizeof *out);
    int i, lo, hi, mid;

    memcpy(p + 1, nums, (size_t)numsSize * sizeof *p);
    qsort(p + 1, (size_t)numsSize, sizeof *p, cmp_int);
    p[0] = 0;
    for (i = 1; i <= numsSize; i++)
        p[i] += p[i - 1];
    for (i = 0; i < queriesSize; i++) {
        /* The answer stays within [lo, hi]. */
        lo = 0;
        hi = numsSize;
        while (lo < hi) {
            mid = lo + (hi - lo + 1) / 2;
            if (p[mid] <= queries[i])
                lo = mid;
            else
                hi = mid - 1;
        }
        out[i] = lo;
    }
    free(p);
    *returnSize = queriesSize;
    return out;
}
