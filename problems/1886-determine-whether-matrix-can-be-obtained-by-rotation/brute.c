/* Slow and obviously right: really spin a copy of mat, moving each cell
 * from row i, column j to row j, column n - 1 - i, and compare the copy
 * with target before each of four quarter turns. */
bool findRotation(int **mat, int matSize, int *matColSize, int **target,
                  int targetSize, int *targetColSize)
{
    int n = matSize, cur[10][10], next[10][10], turn, i, j;
    bool same;

    (void)matColSize;
    (void)targetSize;
    (void)targetColSize;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            cur[i][j] = mat[i][j];
    for (turn = 0; turn < 4; turn++) {
        same = true;
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (cur[i][j] != target[i][j])
                    same = false;
        if (same)
            return true;
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                next[j][n - 1 - i] = cur[i][j];
        memcpy(cur, next, sizeof cur);
    }
    return false;
}
