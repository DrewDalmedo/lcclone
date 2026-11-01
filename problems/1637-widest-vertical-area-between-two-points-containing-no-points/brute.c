/* Slow and obviously right, straight from the definition: try every pair
 * of points as the two sides, and keep the strip only when no point lies
 * strictly between them. */
int maxWidthOfVerticalArea(int **points, int pointsSize, int *pointsColSize)
{
    int i, j, k, a, b, empty, best = 0;

    (void)pointsColSize;
    for (i = 0; i < pointsSize; i++)
        for (j = 0; j < pointsSize; j++) {
            if (i == j || points[i][0] > points[j][0])
                continue;
            a = points[i][0];
            b = points[j][0];
            empty = 1;
            for (k = 0; k < pointsSize; k++)
                if (a < points[k][0] && points[k][0] < b)
                    empty = 0;
            if (empty && b - a > best)
                best = b - a;
        }
    return best;
}
