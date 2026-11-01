/* Slow and obviously right: mark source as reached, then go over all the
 * footbridges again and again, marking both ends of any bridge with one
 * end marked, until a whole pass marks nothing new. */
bool validPath(int n, int **edges, int edgesSize, int *edgesColSize,
               int source, int destination)
{
    char *reached = calloc((size_t)n, 1);
    int changed = 1, i, u, v;
    bool answer;

    (void)edgesColSize;
    reached[source] = 1;
    while (changed) {
        changed = 0;
        for (i = 0; i < edgesSize; i++) {
            u = edges[i][0];
            v = edges[i][1];
            if (reached[u] != reached[v]) {
                reached[u] = reached[v] = 1;
                changed = 1;
            }
        }
    }
    answer = reached[destination];
    free(reached);
    return answer;
}
