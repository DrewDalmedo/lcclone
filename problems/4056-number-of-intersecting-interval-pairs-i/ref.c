/* Reference solution: sort the intervals by start. An interval listed
 * after interval i in that order starts no earlier than i, so the two
 * share a point exactly when it starts at or before the end of i; those
 * later intervals come first, and a binary search counts them. */
static int by_start(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int countIntersectingIntervals(int **intervals, int intervalsSize,
                               int *intervalsColSize)
{
    int n = intervalsSize, i, lo, hi, mid, count = 0;
    int (*iv)[2] = malloc((size_t)n * sizeof *iv);

    (void)intervalsColSize;
    for (i = 0; i < n; i++) {
        iv[i][0] = intervals[i][0];
        iv[i][1] = intervals[i][1];
    }
    qsort(iv, (size_t)n, sizeof *iv, by_start);
    for (i = 0; i < n; i++) {
        /* lo ends at the first later interval that starts past iv[i]'s end. */
        lo = i + 1;
        hi = n;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (iv[mid][0] <= iv[i][1])
                lo = mid + 1;
            else
                hi = mid;
        }
        count += lo - (i + 1);
    }
    free(iv);
    return count;
}
