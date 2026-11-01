/* Slow and obviously right: list every corner the route reaches, the
 * start included, and compare each one with all the corners before it. */
bool isPathCrossing(char *path)
{
    int n = (int)strlen(path), i, k;
    int *xs = malloc((size_t)(n + 1) * sizeof *xs);
    int *ys = malloc((size_t)(n + 1) * sizeof *ys);
    bool crossed = false;

    xs[0] = 0;
    ys[0] = 0;
    for (i = 0; i < n; i++) {
        xs[i + 1] = xs[i] + (path[i] == 'E') - (path[i] == 'W');
        ys[i + 1] = ys[i] + (path[i] == 'N') - (path[i] == 'S');
    }
    for (i = 1; i <= n; i++)
        for (k = 0; k < i; k++)
            if (xs[i] == xs[k] && ys[i] == ys[k])
                crossed = true;
    free(xs);
    free(ys);
    return crossed;
}
