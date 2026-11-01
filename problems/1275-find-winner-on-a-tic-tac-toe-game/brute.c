/* Slow and obviously right, by another route: three different squares of
 * the board lie on one of its eight lines exactly when they are collinear.
 * So for each player, try every three of their squares and test them with
 * a cross product. */
static int has_line(int sq[][2], int n)
{
    int a, b, c;

    for (a = 0; a < n; a++)
        for (b = a + 1; b < n; b++)
            for (c = b + 1; c < n; c++)
                if ((sq[b][0] - sq[a][0]) * (sq[c][1] - sq[a][1])
                    == (sq[b][1] - sq[a][1]) * (sq[c][0] - sq[a][0]))
                    return 1;
    return 0;
}

char *tictactoe(int **moves, int movesSize, int *movesColSize)
{
    int a[5][2], b[5][2], na = 0, nb = 0, i;
    const char *answer;
    char *out;

    (void)movesColSize;
    for (i = 0; i < movesSize; i++) {
        if (i % 2 == 0) {
            a[na][0] = moves[i][0];
            a[na][1] = moves[i][1];
            na++;
        } else {
            b[nb][0] = moves[i][0];
            b[nb][1] = moves[i][1];
            nb++;
        }
    }
    if (has_line(a, na))
        answer = "A";
    else if (has_line(b, nb))
        answer = "B";
    else if (movesSize == 9)
        answer = "Draw";
    else
        answer = "Pending";
    out = malloc(strlen(answer) + 1);
    strcpy(out, answer);
    return out;
}
