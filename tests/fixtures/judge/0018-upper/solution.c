void upper(char** grid, int gridSize, int* gridColSize) {
    int i, j;
    for (i = 0; i < gridSize; i++)
        for (j = 0; j < gridColSize[i]; j++)
            grid[i][j] = (char)toupper((unsigned char)grid[i][j]);
}
