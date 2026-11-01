/* Reference solution: one pass over the points, skipping those that share
 * neither coordinate with (x, y) and keeping the first point seen at the
 * smallest distance so far (a strictly smaller distance replaces it). */
int nearestValidPoint(int x, int y, int **points, int pointsSize,
                      int *pointsColSize)
{
    int best = -1, bestDist = 0, i, a, b, d;

    (void)pointsColSize;
    for (i = 0; i < pointsSize; i++) {
        a = points[i][0];
        b = points[i][1];
        if (a != x && b != y)
            continue;
        d = abs(a - x) + abs(b - y);
        if (best < 0 || d < bestDist) {
            best = i;
            bestDist = d;
        }
    }
    return best;
}
