/* Right, but too slow at full size: sort the x-coordinates by insertion,
 * then take the widest gap. The insertion sort is O(n^2) unless they come
 * nearly in order: about 2.5 * 10^9 moves for 10^5 points in random
 * order, and twice that when they come in decreasing order. */
int maxWidthOfVerticalArea(int **points, int pointsSize, int *pointsColSize)
{
    int *xs = malloc((size_t)pointsSize * sizeof *xs);
    int i, j, v, best = 0;

    (void)pointsColSize;
    for (i = 0; i < pointsSize; i++) {
        v = points[i][0];
        for (j = i; j > 0 && xs[j - 1] > v; j--)
            xs[j] = xs[j - 1];
        xs[j] = v;
    }
    for (i = 1; i < pointsSize; i++)
        if (xs[i] - xs[i - 1] > best)
            best = xs[i] - xs[i - 1];
    free(xs);
    return best;
}
