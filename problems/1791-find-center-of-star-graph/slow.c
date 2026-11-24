/* Right, but too slow at full size: for each node 1, 2, ... count the
 * edges it lies on, and stop at the one that lies on all of them. That is
 * O(n^2) unless the center comes early: about 10^10 steps for the center
 * n at n = 10^5. */
int findCenter(int **edges, int edgesSize, int *edgesColSize)
{
    int n = edgesSize + 1, c, i, deg;

    (void)edgesColSize;
    for (c = 1; c <= n; c++) {
        deg = 0;
        for (i = 0; i < edgesSize; i++)
            if (edges[i][0] == c || edges[i][1] == c)
                deg++;
        if (deg == edgesSize)
            return c;
    }
    return -1;
}
