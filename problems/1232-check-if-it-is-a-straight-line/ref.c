/* Reference solution: the first two points give a direction (dx, dy). A
 * point lies on their line exactly when its offset from the first point
 * is parallel to (dx, dy), that is when the cross product is 0. Integers
 * only, no division: the products stay below 4 * 10^8. */
bool checkStraightLine(int **coordinates, int coordinatesSize,
                       int *coordinatesColSize)
{
    int x0 = coordinates[0][0], y0 = coordinates[0][1];
    int dx = coordinates[1][0] - x0, dy = coordinates[1][1] - y0, i;

    (void)coordinatesColSize;
    for (i = 2; i < coordinatesSize; i++)
        if (dx * (coordinates[i][1] - y0) != dy * (coordinates[i][0] - x0))
            return false;
    return true;
}
