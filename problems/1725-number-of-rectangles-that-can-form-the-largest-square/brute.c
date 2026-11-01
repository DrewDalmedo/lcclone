/* Slow and obviously right: a sheet counts when it can also be cut into
 * the biggest square of every sheet, i.e. when each sheet's shorter side
 * k satisfies k <= l and k <= w for this sheet. */
int countGoodRectangles(int **rectangles, int rectanglesSize,
                        int *rectanglesColSize)
{
    int count = 0, i, j, k, ok;

    (void)rectanglesColSize;
    for (i = 0; i < rectanglesSize; i++) {
        ok = 1;
        for (j = 0; j < rectanglesSize; j++) {
            k = rectangles[j][0];
            if (rectangles[j][1] < k)
                k = rectangles[j][1];
            if (k > rectangles[i][0] || k > rectangles[i][1])
                ok = 0;
        }
        count += ok;
    }
    return count;
}
