/* Reference solution: find the rook, then walk each of the four directions
 * away from it over empty squares, stopping at the first piece or the
 * edge. Count the directions that stop at a pawn. */
int numRookCaptures(char **board, int boardSize, int *boardColSize)
{
    static const int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
    int rook_r = 0, rook_c = 0, count = 0, i, j, d, r, c;

    for (i = 0; i < boardSize; i++)
        for (j = 0; j < boardColSize[i]; j++)
            if (board[i][j] == 'R') {
                rook_r = i;
                rook_c = j;
            }
    for (d = 0; d < 4; d++) {
        r = rook_r + dr[d];
        c = rook_c + dc[d];
        while (r >= 0 && r < boardSize && c >= 0 && c < boardColSize[r]
               && board[r][c] == '.') {
            r += dr[d];
            c += dc[d];
        }
        if (r >= 0 && r < boardSize && c >= 0 && c < boardColSize[r]
            && board[r][c] == 'p')
            count++;
    }
    return count;
}
