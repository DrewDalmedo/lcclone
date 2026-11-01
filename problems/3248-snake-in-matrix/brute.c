/* Slow and obviously right: write the labels onto an n x n board in
 * reading order, walk the snake's row and column by comparing each command
 * in full, and read the label of the square it ends on. */
int finalPositionOfSnake(int n, char **commands, int commandsSize)
{
    int board[10][10], r, c, next = 0, i;

    for (r = 0; r < n; r++)
        for (c = 0; c < n; c++)
            board[r][c] = next++;
    r = 0;
    c = 0;
    for (i = 0; i < commandsSize; i++) {
        if (strcmp(commands[i], "UP") == 0)
            r--;
        else if (strcmp(commands[i], "DOWN") == 0)
            r++;
        else if (strcmp(commands[i], "LEFT") == 0)
            c--;
        else if (strcmp(commands[i], "RIGHT") == 0)
            c++;
    }
    return board[r][c];
}
