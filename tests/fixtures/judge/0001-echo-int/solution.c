int** echo(int** x, int xSize, int* xColSize, int* returnSize, int** returnColumnSizes) {
    *returnSize = xSize;
    *returnColumnSizes = xColSize;
    return x;
}
