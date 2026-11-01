/* Reference solution: two cells lie on the same slant exactly when one is
 * the next step from the other, down and to the right, or a chain of such
 * steps. So every slant holds one number iff every cell outside the top row
 * and the left column equals the cell up and to the left of it. */
bool isToeplitzMatrix(int **matrix, int matrixSize, int *matrixColSize)
{
    int m = matrixSize, n = matrixColSize[0], i, j;

    for (i = 1; i < m; i++)
        for (j = 1; j < n; j++)
            if (matrix[i][j] != matrix[i - 1][j - 1])
                return false;
    return true;
}
