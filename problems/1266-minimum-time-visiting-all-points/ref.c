/* Reference solution: each move changes x and y by at most 1 each, so
 * going from one point to the next takes at least max(|dx|, |dy|)
 * seconds; diagonal moves cover the smaller difference while the larger
 * one is being covered, so that is also enough. Add it up over the
 * consecutive pairs. */
int minTimeToVisitAllPoints(int **points, int pointsSize,
                            int *pointsColSize)
{
    int total = 0, i, dx, dy;

    (void)pointsColSize;
    for (i = 1; i < pointsSize; i++) {
        dx = abs(points[i][0] - points[i - 1][0]);
        dy = abs(points[i][1] - points[i - 1][1]);
        total += dx > dy ? dx : dy;
    }
    return total;
}
