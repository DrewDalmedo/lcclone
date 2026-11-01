/* Reference solution: on its own, a stack of v > 0 cubes has 4v + 2 faces
 * to paint (4 sides of v squares, a top and a bottom). Two stacks of
 * heights a and b in cells that share a side press min(a, b) faces of each
 * against the other, so each such pair, met once by looking only right and
 * down, takes away 2 * min(a, b). */
static int smaller(int a, int b)
{
    return a < b ? a : b;
}

int surfaceArea(int **grid, int gridSize, int *gridColSize)
{
    int n = gridSize, area = 0, i, j, v;

    (void)gridColSize;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            v = grid[i][j];
            if (v == 0)
                continue;
            area += 4 * v + 2;
            if (j + 1 < n)
                area -= 2 * smaller(v, grid[i][j + 1]);
            if (i + 1 < n)
                area -= 2 * smaller(v, grid[i + 1][j]);
        }
    return area;
}
