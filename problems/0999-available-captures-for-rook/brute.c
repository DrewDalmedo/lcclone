/* Slow and obviously right: go over every pawn on the board, and count it
 * when it is in the rook's row or column and every square strictly between
 * the two is empty. */
int numRookCaptures(char **board, int boardSize, int *boardColSize)
{
    int rook_r = 0, rook_c = 0, count = 0, clear, lo, hi, i, j, k;

    for (i = 0; i < boardSize; i++)
        for (j = 0; j < boardColSize[i]; j++)
            if (board[i][j] == 'R') {
                rook_r = i;
                rook_c = j;
            }
    for (i = 0; i < boardSize; i++)
        for (j = 0; j < boardColSize[i]; j++) {
            if (board[i][j] != 'p' || (i != rook_r && j != rook_c))
                continue;
            clear = 1;
            if (i == rook_r) {
                lo = j < rook_c ? j : rook_c;
                hi = j < rook_c ? rook_c : j;
                for (k = lo + 1; k < hi; k++)
                    if (board[i][k] != '.')
                        clear = 0;
            } else {
                lo = i < rook_r ? i : rook_r;
                hi = i < rook_r ? rook_r : i;
                for (k = lo + 1; k < hi; k++)
                    if (board[k][j] != '.')
                        clear = 0;
            }
            count += clear;
        }
    return count;
}
