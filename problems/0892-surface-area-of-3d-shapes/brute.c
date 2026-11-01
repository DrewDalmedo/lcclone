/* Slow and obviously right: visit every cube, one at a time, and count
 * those of its 6 faces whose neighbouring spot holds no cube. */
static int filled(int **grid, int n, int i, int j, int h)
{
    return i >= 0 && i < n && j >= 0 && j < n && h >= 0 && h < grid[i][j];
}

int surfaceArea(int **grid, int gridSize, int *gridColSize)
{
    static const int di[] = {1, -1, 0, 0, 0, 0};
    static const int dj[] = {0, 0, 1, -1, 0, 0};
    static const int dh[] = {0, 0, 0, 0, 1, -1};
    int n = gridSize, area = 0, i, j, h, d;

    (void)gridColSize;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            for (h = 0; h < grid[i][j]; h++)
                for (d = 0; d < 6; d++)
                    if (!filled(grid, n, i + di[d], j + dj[d], h + dh[d]))
                        area++;
    return area;
}
