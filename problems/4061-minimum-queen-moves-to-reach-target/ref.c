/* Reference solution: no move for the same square; one move when the two
 * squares share a row, a column or a diagonal (as many rows apart as
 * columns apart); otherwise two, along the row and then the column. */
int minQueenMoves(int *source, int sourceSize, int *target, int targetSize)
{
    int dr = abs(source[0] - target[0]), dc = abs(source[1] - target[1]);

    (void)sourceSize;
    (void)targetSize;
    if (dr == 0 && dc == 0)
        return 0;
    if (dr == 0 || dc == 0 || dr == dc)
        return 1;
    return 2;
}
