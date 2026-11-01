/* Slow and obviously right: for every row, every column and every value
 * from 1 to n, look for the value in that line. */
bool checkValid(int **matrix, int matrixSize, int *matrixColSize)
{
    int n = matrixSize, line, v, k, inRow, inCol;

    (void)matrixColSize;
    for (line = 0; line < n; line++)
        for (v = 1; v <= n; v++) {
            inRow = 0;
            inCol = 0;
            for (k = 0; k < n; k++) {
                if (matrix[line][k] == v)
                    inRow = 1;
                if (matrix[k][line] == v)
                    inCol = 1;
            }
            if (!inRow || !inCol)
                return false;
        }
    return true;
}
