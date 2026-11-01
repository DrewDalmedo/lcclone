/* Slow and obviously right: every triple of points, with the shoelace
 * formula for twice the area. */
double largestTriangleArea(int **points, int pointsSize, int *pointsColSize)
{
    int best = 0, i, j, k;

    (void)pointsColSize;
    for (i = 0; i < pointsSize; i++)
        for (j = i + 1; j < pointsSize; j++)
            for (k = j + 1; k < pointsSize; k++) {
                int *a = points[i], *b = points[j], *c = points[k];
                int twice = a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) +
                            c[0] * (a[1] - b[1]);

                if (twice < 0)
                    twice = -twice;
                if (twice > best)
                    best = twice;
            }
    return best / 2.0;
}
