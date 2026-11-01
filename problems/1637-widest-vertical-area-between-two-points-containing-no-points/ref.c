/* Reference solution: copy the x-coordinates into a flat array, sort it,
 * and return the largest difference between neighbours. The y-coordinates
 * play no part. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int maxWidthOfVerticalArea(int **points, int pointsSize, int *pointsColSize)
{
    int *xs = malloc((size_t)pointsSize * sizeof *xs);
    int i, best = 0;

    (void)pointsColSize;
    for (i = 0; i < pointsSize; i++)
        xs[i] = points[i][0];
    qsort(xs, (size_t)pointsSize, sizeof *xs, cmp_int);
    for (i = 1; i < pointsSize; i++)
        if (xs[i] - xs[i - 1] > best)
            best = xs[i] - xs[i - 1];
    free(xs);
    return best;
}
