/* Reference solution: the degree of vertex i is the number of 1s in row
 * i, since each 1 there is one edge from i to another vertex. */
int *findDegrees(int **matrix, int matrixSize, int *matrixColSize,
                 int *returnSize)
{
    int n = matrixSize, i, j;
    int *deg = malloc((size_t)n * sizeof *deg);

    (void)matrixColSize;
    for (i = 0; i < n; i++) {
        deg[i] = 0;
        for (j = 0; j < n; j++)
            deg[i] += matrix[i][j];
    }
    *returnSize = n;
    return deg;
}
