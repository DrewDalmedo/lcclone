/* Reference solution: one pass. Each sheet's biggest square has the side
 * min(l, w); keep the largest side seen so far and how many sheets reach
 * it, starting the count again at 1 when a larger side turns up. */
int countGoodRectangles(int **rectangles, int rectanglesSize,
                        int *rectanglesColSize)
{
    int best = 0, count = 0, i, side;

    (void)rectanglesColSize;
    for (i = 0; i < rectanglesSize; i++) {
        side = rectangles[i][0] < rectangles[i][1] ? rectangles[i][0]
                                                   : rectangles[i][1];
        if (side > best) {
            best = side;
            count = 1;
        } else if (side == best) {
            count++;
        }
    }
    return count;
}
