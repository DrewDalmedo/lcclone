/* Reference solution: add every row's weight into a table indexed by
 * label (1..1000), then read the table from the smallest label up and
 * emit a row for each label that got any weight (every weight is >= 1). */
int **mergeSimilarItems(int **items1, int items1Size, int *items1ColSize,
                        int **items2, int items2Size, int *items2ColSize,
                        int *returnSize, int **returnColumnSizes)
{
    int total[1001] = {0};
    int rows = 0, k = 0, v, i;
    int **out;

    (void)items1ColSize;
    (void)items2ColSize;
    for (i = 0; i < items1Size; i++)
        total[items1[i][0]] += items1[i][1];
    for (i = 0; i < items2Size; i++)
        total[items2[i][0]] += items2[i][1];
    for (v = 1; v <= 1000; v++)
        if (total[v] > 0)
            rows++;
    out = malloc((size_t)rows * sizeof *out);
    *returnColumnSizes = malloc((size_t)rows * sizeof **returnColumnSizes);
    for (v = 1; v <= 1000; v++)
        if (total[v] > 0) {
            out[k] = malloc(2 * sizeof **out);
            out[k][0] = v;
            out[k][1] = total[v];
            (*returnColumnSizes)[k] = 2;
            k++;
        }
    *returnSize = rows;
    return out;
}
