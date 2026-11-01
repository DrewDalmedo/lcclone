/* Slow and obviously right: again and again, find the smallest label above
 * the last one written, looking at every row of both lists, and add up the
 * weights of all rows that carry it. */
int **mergeSimilarItems(int **items1, int items1Size, int *items1ColSize,
                        int **items2, int items2Size, int *items2ColSize,
                        int *returnSize, int **returnColumnSizes)
{
    int **out = malloc((size_t)(items1Size + items2Size) * sizeof *out);
    int last = 0, rows = 0, next, sum, i;

    (void)items1ColSize;
    (void)items2ColSize;
    *returnColumnSizes =
        malloc((size_t)(items1Size + items2Size) * sizeof **returnColumnSizes);
    for (;;) {
        next = 0;
        for (i = 0; i < items1Size; i++)
            if (items1[i][0] > last && (next == 0 || items1[i][0] < next))
                next = items1[i][0];
        for (i = 0; i < items2Size; i++)
            if (items2[i][0] > last && (next == 0 || items2[i][0] < next))
                next = items2[i][0];
        if (next == 0)
            break;
        sum = 0;
        for (i = 0; i < items1Size; i++)
            if (items1[i][0] == next)
                sum += items1[i][1];
        for (i = 0; i < items2Size; i++)
            if (items2[i][0] == next)
                sum += items2[i][1];
        out[rows] = malloc(2 * sizeof **out);
        out[rows][0] = next;
        out[rows][1] = sum;
        (*returnColumnSizes)[rows] = 2;
        rows++;
        last = next;
    }
    *returnSize = rows;
    return out;
}
