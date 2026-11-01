/* Reference solution: the cross product of the vectors from the first
 * point to the other two is twice the signed area of the triangle. It is
 * zero exactly when the points are on one line, which includes two of
 * them being the same point. */
bool isBoomerang(int **points, int pointsSize, int *pointsColSize)
{
    int ax = points[0][0], ay = points[0][1];
    int ux = points[1][0] - ax, uy = points[1][1] - ay;
    int vx = points[2][0] - ax, vy = points[2][1] - ay;

    (void)pointsSize;
    (void)pointsColSize;
    return ux * vy - uy * vx != 0;
}
