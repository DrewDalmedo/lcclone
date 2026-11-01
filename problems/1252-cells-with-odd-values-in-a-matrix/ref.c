/* Reference solution: square (i, j) ends up at (steps on row i) + (steps
 * on column j), which is odd exactly when one of the two counts is odd and
 * the other even. So keep only the parity of each row and each column:
 * with oddR odd rows and oddC odd columns, the odd squares are the odd
 * rows crossed with the even columns, plus the even rows crossed with the
 * odd columns. */
int oddCells(int m, int n, int **indices, int indicesSize,
             int *indicesColSize)
{
    char row[50] = {0}, col[50] = {0};
    int oddR = 0, oddC = 0, i;

    (void)indicesColSize;
    for (i = 0; i < indicesSize; i++) {
        row[indices[i][0]] ^= 1;
        col[indices[i][1]] ^= 1;
    }
    for (i = 0; i < m; i++)
        oddR += row[i];
    for (i = 0; i < n; i++)
        oddC += col[i];
    return oddR * (n - oddC) + (m - oddR) * oddC;
}
