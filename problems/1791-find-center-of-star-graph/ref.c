/* Reference solution: the center lies on every edge, and every other node
 * on just one, so the center is the node that the first two edges share.
 * O(1): no other edge needs to be read. */
int findCenter(int **edges, int edgesSize, int *edgesColSize)
{
    int a = edges[0][0];

    (void)edgesSize;
    (void)edgesColSize;
    if (a == edges[1][0] || a == edges[1][1])
        return a;
    return edges[0][1];
}
