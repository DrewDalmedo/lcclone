/* Slow and obviously right: first look for two points at the same place.
 * Otherwise walk along the line through the first two points, one
 * whole-number point at a time (the smallest step between two of them is
 * their difference divided by its gcd), across the whole 0..100 square in
 * both directions, and see whether the walk lands on the third point. */
static int gcd(int a, int b)
{
    while (b) {
        int t = a % b;

        a = b;
        b = t;
    }
    return a;
}

bool isBoomerang(int **points, int pointsSize, int *pointsColSize)
{
    int i, k, g, sx, sy, t;

    (void)pointsColSize;
    for (i = 0; i < pointsSize; i++)
        for (k = i + 1; k < pointsSize; k++)
            if (points[i][0] == points[k][0] && points[i][1] == points[k][1])
                return false;
    sx = points[1][0] - points[0][0];
    sy = points[1][1] - points[0][1];
    g = gcd(sx < 0 ? -sx : sx, sy < 0 ? -sy : sy);
    sx /= g;
    sy /= g;
    for (t = -101; t <= 101; t++)
        if (points[0][0] + t * sx == points[2][0]
            && points[0][1] + t * sy == points[2][1])
            return false;
    return true;
}
