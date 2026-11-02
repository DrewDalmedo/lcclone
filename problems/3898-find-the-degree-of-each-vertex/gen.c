/* A symmetric 0/1 matrix with a zero diagonal. Small cases have 1 to 6
 * vertices; full-size ones have 100. The seed picks the graph: no edges,
 * every edge, a star around a random center, a path through the vertices
 * in a random order, or each edge present with a chance the seed also
 * picks (10, 50 or 90 in 100). */
void gen(int small)
{
    int n = small ? (int)rnd(1, 6) : 100;
    int shape = (int)rnd(0, 6), i, j, c, e;
    int *g = calloc((size_t)n * n, sizeof *g);
    int *order = malloc((size_t)n * sizeof *order);

    for (i = 0; i < n; i++)
        order[i] = i;
    rnd_shuffle(order, n);
    c = order[0];
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++) {
            if (shape == 0)
                e = 0;
            else if (shape == 1)
                e = 1;
            else if (shape == 2)
                e = i == c || j == c;
            else if (shape == 3)
                e = 0;
            else
                e = rnd_chance(shape == 4 ? 10 : shape == 5 ? 50 : 90);
            g[i * n + j] = g[j * n + i] = e;
        }
    if (shape == 3)
        for (i = 0; i + 1 < n; i++) {
            g[order[i] * n + order[i + 1]] = 1;
            g[order[i + 1] * n + order[i]] = 1;
        }
    put_int_grid(g, n, n);
    free(order);
    free(g);
}
