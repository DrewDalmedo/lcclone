/* Slow and obviously right, straight from the definition: try each node
 * 1..n in turn and return the one that lies on every edge. */
int findCenter(int **edges, int edgesSize, int *edgesColSize)
{
    int n = edgesSize + 1, c, i;

    (void)edgesColSize;
    for (c = 1; c <= n; c++) {
        for (i = 0; i < edgesSize; i++)
            if (edges[i][0] != c && edges[i][1] != c)
                break;
        if (i == edgesSize)
            return c;
    }
    return -1;
}
