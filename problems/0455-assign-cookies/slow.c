/* Right, but too slow at full size: for each child, look through all the
 * cookies for the smallest unused one it is content with. */
int findContentChildren(int *g, int gSize, int *s, int sSize)
{
    char *used = calloc((size_t)sSize + 1, 1);
    int content = 0, i, j, best;

    for (i = 0; i < gSize; i++) {
        best = -1;
        for (j = 0; j < sSize; j++)
            if (!used[j] && s[j] >= g[i] && (best < 0 || s[j] < s[best]))
                best = j;
        if (best >= 0) {
            used[best] = 1;
            content++;
        }
    }
    free(used);
    return content;
}
