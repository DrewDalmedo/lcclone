/* Slow and obviously right: for every 1, look along its whole row and its
 * whole column for another 1. */
int numSpecial(int **mat, int matSize, int *matColSize)
{
    int m = matSize, n = matColSize[0], count = 0, i, j, t, alone;

    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++) {
            if (mat[i][j] != 1)
                continue;
            alone = 1;
            for (t = 0; t < n; t++)
                if (t != j && mat[i][t] == 1)
                    alone = 0;
            for (t = 0; t < m; t++)
                if (t != i && mat[t][j] == 1)
                    alone = 0;
            count += alone;
        }
    return count;
}
