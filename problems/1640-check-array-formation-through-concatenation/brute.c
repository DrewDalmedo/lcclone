/* Slow and obviously right: try every order of the pieces, lay them end to
 * end, and compare the result with arr. */
static int try_orders(const int *arr, int n, int **pieces, int m,
                      const int *len, int *order, int k)
{
    int i, j, t, pos, ok;

    if (k == m) {
        pos = 0;
        for (i = 0; i < m; i++)
            for (j = 0; j < len[order[i]]; j++) {
                if (pos >= n || arr[pos] != pieces[order[i]][j])
                    return 0;
                pos++;
            }
        return pos == n;
    }
    for (i = k; i < m; i++) {
        t = order[k];
        order[k] = order[i];
        order[i] = t;
        ok = try_orders(arr, n, pieces, m, len, order, k + 1);
        order[i] = order[k];
        order[k] = t;
        if (ok)
            return 1;
    }
    return 0;
}

bool canFormArray(int *arr, int arrSize, int **pieces, int piecesSize,
                  int *piecesColSize)
{
    int *order = malloc((size_t)piecesSize * sizeof *order);
    int i, ok;

    for (i = 0; i < piecesSize; i++)
        order[i] = i;
    ok = try_orders(arr, arrSize, pieces, piecesSize, piecesColSize, order, 0);
    free(order);
    return ok;
}
