/* Reference solution: one pass. Diagonals are compared by their squares
 * l*l + w*w, which are exact integers and in the same order; keep the
 * longest seen so far and the largest area among the rectangles that
 * reach it. */
int areaOfMaxDiagonal(int **dimensions, int dimensionsSize,
                      int *dimensionsColSize)
{
    int best = 0, area = 0, i, l, w, d;

    (void)dimensionsColSize;
    for (i = 0; i < dimensionsSize; i++) {
        l = dimensions[i][0];
        w = dimensions[i][1];
        d = l * l + w * w;
        if (d > best || (d == best && l * w > area)) {
            best = d;
            area = l * w;
        }
    }
    return area;
}
