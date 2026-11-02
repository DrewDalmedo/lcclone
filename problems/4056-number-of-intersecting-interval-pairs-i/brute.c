/* Slow and obviously right: two intervals intersect when some point from
 * 0 to 100 lies in both. Try every pair, and every point for each. */
int countIntersectingIntervals(int **intervals, int intervalsSize,
                               int *intervalsColSize)
{
    int i, j, x, count = 0;

    (void)intervalsColSize;
    for (i = 0; i < intervalsSize; i++)
        for (j = i + 1; j < intervalsSize; j++)
            for (x = 0; x <= 100; x++)
                if (intervals[i][0] <= x && x <= intervals[i][1] &&
                    intervals[j][0] <= x && x <= intervals[j][1]) {
                    count++;
                    break;
                }
    return count;
}
