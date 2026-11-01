/* Slow and obviously right: two cells (i, j) and (k, l) are on the same
 * slant when j - i == l - k. Compare every such pair of cells. */
bool isToeplitzMatrix(int **matrix, int matrixSize, int *matrixColSize)
{
    int m = matrixSize, n = matrixColSize[0], i, j, k, l;

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            for (k = 0; k < m; k++)
                for (l = 0; l < n; l++)
                    if (j - i == l - k && matrix[i][j] != matrix[k][l])
                        return false;
    return true;
}
