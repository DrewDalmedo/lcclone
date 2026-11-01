/* Slow and obviously right: try every way of handing out the cookies.
 * Child i either gets no cookie or any unused cookie it is content with;
 * keep the best count. Exponential, so only for small cases. */
static int best_from(int i, const int *g, int gSize, const int *s, int sSize,
                     char *used)
{
    int best, got, j;

    if (i == gSize)
        return 0;
    best = best_from(i + 1, g, gSize, s, sSize, used); /* no cookie */
    for (j = 0; j < sSize; j++)
        if (!used[j] && s[j] >= g[i]) {
            used[j] = 1;
            got = 1 + best_from(i + 1, g, gSize, s, sSize, used);
            used[j] = 0;
            if (got > best)
                best = got;
        }
    return best;
}

int findContentChildren(int *g, int gSize, int *s, int sSize)
{
    char *used = calloc((size_t)sSize + 1, 1);
    int best = best_from(0, g, gSize, s, sSize, used);

    free(used);
    return best;
}
