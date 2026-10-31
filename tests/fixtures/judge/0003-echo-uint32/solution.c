uint32_t** echo(uint32_t** x, int xSize, int* xColSize, int* returnSize, int** returnColumnSizes) {
    *returnSize = xSize;
    *returnColumnSizes = xColSize;
    return x;
}
