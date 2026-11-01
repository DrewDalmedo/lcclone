/* Reference solution: keep a running score for each of the eight lines,
 * adding 1 for a square of A's and subtracting 1 for one of B's. The
 * centre counts for both diagonals. A line reaches 3 or -3 only when one
 * player owns all of it. With no win, a full board is a draw. */
char *tictactoe(int **moves, int movesSize, int *movesColSize)
{
    int row[3] = {0}, col[3] = {0}, diag = 0, anti = 0, i, r, c, v;
    const char *answer = movesSize == 9 ? "Draw" : "Pending";
    char *out;

    (void)movesColSize;
    for (i = 0; i < movesSize; i++) {
        r = moves[i][0];
        c = moves[i][1];
        v = i % 2 == 0 ? 1 : -1;
        row[r] += v;
        col[c] += v;
        if (r == c)
            diag += v;
        if (r + c == 2)
            anti += v;
        if (abs(row[r]) == 3 || abs(col[c]) == 3 || abs(diag) == 3
            || abs(anti) == 3) {
            answer = v == 1 ? "A" : "B";
            break;
        }
    }
    out = malloc(strlen(answer) + 1);
    strcpy(out, answer);
    return out;
}
