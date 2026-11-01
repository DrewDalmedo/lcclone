/* Reference solution: union-find. Each footbridge merges the groups of its
 * two islands (the smaller group joins the larger, and finding a group
 * halves the path it walks); then source can reach destination exactly
 * when both are in the same group. With no footbridges, edges is never
 * touched, so it may be NULL. */
static int find_group(int *parent, int x)
{
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

bool validPath(int n, int **edges, int edgesSize, int *edgesColSize,
               int source, int destination)
{
    int *parent = malloc((size_t)n * sizeof *parent);
    int *size = malloc((size_t)n * sizeof *size);
    int i, a, b, t;
    bool same;

    (void)edgesColSize;
    for (i = 0; i < n; i++) {
        parent[i] = i;
        size[i] = 1;
    }
    for (i = 0; i < edgesSize; i++) {
        a = find_group(parent, edges[i][0]);
        b = find_group(parent, edges[i][1]);
        if (a == b)
            continue;
        if (size[a] < size[b]) {
            t = a;
            a = b;
            b = t;
        }
        parent[b] = a;
        size[a] += size[b];
    }
    same = find_group(parent, source) == find_group(parent, destination);
    free(parent);
    free(size);
    return same;
}
