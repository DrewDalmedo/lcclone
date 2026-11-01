/* Leaves a row NULL, though its size says it has elements. */
void upper(char** grid, int gridSize, int* gridColSize) {
    (void)gridColSize;
    if (gridSize > 0)
        grid[0] = NULL;
}
