/* Slow and obviously right: the points lie on one line exactly when every
 * three of them do, and three points do when the triangle they form has
 * no area. Twice its signed area comes straight from the coordinates. */
static long long area2(const int *a, const int *b, const int *c)
{
    return (long long)a[0] * (b[1] - c[1]) + (long long)b[0] * (c[1] - a[1])
         + (long long)c[0] * (a[1] - b[1]);
}

bool checkStraightLine(int **coordinates, int coordinatesSize,
                       int *coordinatesColSize)
{
    int i, j, k;

    (void)coordinatesColSize;
    for (i = 0; i < coordinatesSize; i++)
        for (j = i + 1; j < coordinatesSize; j++)
            for (k = j + 1; k < coordinatesSize; k++)
                if (area2(coordinates[i], coordinates[j], coordinates[k]) != 0)
                    return false;
    return true;
}
