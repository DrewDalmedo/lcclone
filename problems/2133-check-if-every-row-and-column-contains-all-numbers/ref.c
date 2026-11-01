/* Reference solution: every value lies in 1..n, so a line is complete
 * exactly when no value repeats in it. A table indexed by value records
 * the number of the line in which each value was last seen, so it never
 * needs clearing between lines. */
bool checkValid(int **matrix, int matrixSize, int *matrixColSize)
{
    int n = matrixSize, i, j, v, line;
    int *seen = calloc((size_t)n + 1, sizeof *seen);
    bool ok = true;

    (void)matrixColSize;
    for (i = 0; i < n && ok; i++) {
        line = i + 1;
        for (j = 0; j < n && ok; j++) {
            v = matrix[i][j];
            if (seen[v] == line)
                ok = false;
            seen[v] = line;
        }
    }
    for (j = 0; j < n && ok; j++) {
        line = n + j + 1;
        for (i = 0; i < n && ok; i++) {
            v = matrix[i][j];
            if (seen[v] == line)
                ok = false;
            seen[v] = line;
        }
    }
    free(seen);
    return ok;
}
