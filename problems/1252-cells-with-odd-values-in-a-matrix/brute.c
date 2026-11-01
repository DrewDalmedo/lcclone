/* Slow and obviously right: fill in the board square by square, one step
 * at a time (the crossing square gets both additions), then count the odd
 * squares. */
int oddCells(int m, int n, int **indices, int indicesSize,
             int *indicesColSize)
{
    int board[50][50] = {{0}};
    int count = 0, s, i, j;

    (void)indicesColSize;
    for (s = 0; s < indicesSize; s++) {
        for (j = 0; j < n; j++)
            board[indices[s][0]][j]++;
        for (i = 0; i < m; i++)
            board[i][indices[s][1]]++;
    }
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            if (board[i][j] % 2 == 1)
                count++;
    return count;
}
