/* Slow and obviously right, straight from the definition: return the
 * first in-line point that no in-line point beats, where a point beats
 * another when it is nearer, or as near with a smaller index. */
static int inLine(int x, int y, const int *p)
{
    return p[0] == x || p[1] == y;
}

static int dist(int x, int y, const int *p)
{
    int dx = p[0] > x ? p[0] - x : x - p[0];
    int dy = p[1] > y ? p[1] - y : y - p[1];

    return dx + dy;
}

int nearestValidPoint(int x, int y, int **points, int pointsSize,
                      int *pointsColSize)
{
    int i, j, beaten;

    (void)pointsColSize;
    for (i = 0; i < pointsSize; i++) {
        if (!inLine(x, y, points[i]))
            continue;
        beaten = 0;
        for (j = 0; j < pointsSize; j++)
            if (j != i && inLine(x, y, points[j]) &&
                (dist(x, y, points[j]) < dist(x, y, points[i]) ||
                 (dist(x, y, points[j]) == dist(x, y, points[i]) && j < i)))
                beaten = 1;
        if (!beaten)
            return i;
    }
    return -1;
}
