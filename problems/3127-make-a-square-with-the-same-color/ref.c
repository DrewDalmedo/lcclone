/* Reference solution: a block can be made one color with at most one
 * repaint exactly when at least three of its tiles share a color, that is,
 * when its count of black tiles is anything but 2. Check the four blocks. */
bool canMakeSquare(char **grid, int gridSize, int *gridColSize)
{
    int r, c, black;

    (void)gridSize;
    (void)gridColSize;
    for (r = 0; r < 2; r++)
        for (c = 0; c < 2; c++) {
            black = (grid[r][c] == 'B') + (grid[r][c + 1] == 'B') +
                    (grid[r + 1][c] == 'B') + (grid[r + 1][c + 1] == 'B');
            if (black != 2)
                return true;
        }
    return false;
}
