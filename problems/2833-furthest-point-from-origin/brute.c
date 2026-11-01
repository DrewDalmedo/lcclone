/* Slow and obviously right: keep the set of every position the token can
 * be at after each step (a blank leads both ways), and at the end take the
 * reachable position farthest from 0. */
int furthestDistanceFromOrigin(char *moves)
{
    int n = (int)strlen(moves), size = 2 * n + 1, i, p, best = 0;
    char *can = calloc((size_t)size, 1), *next = malloc((size_t)size), *t;

    can[n] = 1;                         /* index n is position 0 */
    for (i = 0; i < n; i++) {
        memset(next, 0, (size_t)size);
        for (p = 0; p < size; p++) {
            if (!can[p])
                continue;
            if (moves[i] != 'R' && p > 0)
                next[p - 1] = 1;
            if (moves[i] != 'L' && p + 1 < size)
                next[p + 1] = 1;
        }
        t = can;
        can = next;
        next = t;
    }
    for (p = 0; p < size; p++)
        if (can[p] && (p > n ? p - n : n - p) > best)
            best = p > n ? p - n : n - p;
    free(can);
    free(next);
    return best;
}
