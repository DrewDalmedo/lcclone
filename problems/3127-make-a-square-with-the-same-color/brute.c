/* Slow and obviously right: try leaving the board alone and every one of
 * the nine repaints in turn, on a copy of the board, and look for a block
 * of four equal tiles after each. */
static bool has_uniform_block(char b[3][3])
{
    int r, c;

    for (r = 0; r < 2; r++)
        for (c = 0; c < 2; c++)
            if (b[r][c] == b[r][c + 1] && b[r][c] == b[r + 1][c] &&
                b[r][c] == b[r + 1][c + 1])
                return true;
    return false;
}

bool canMakeSquare(char **grid, int gridSize, int *gridColSize)
{
    char b[3][3];
    int choice, r, c;

    (void)gridSize;
    (void)gridColSize;
    for (choice = -1; choice < 9; choice++) {
        for (r = 0; r < 3; r++)
            for (c = 0; c < 3; c++)
                b[r][c] = grid[r][c];
        if (choice >= 0) {
            r = choice / 3;
            c = choice % 3;
            b[r][c] = b[r][c] == 'B' ? 'W' : 'B';
        }
        if (has_uniform_block(b))
            return true;
    }
    return false;
}
