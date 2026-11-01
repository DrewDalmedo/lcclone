/* Reference solution: one pass. A group ends at the last letter or where
 * the next letter differs; when it does, record [start, i] if it has at
 * least 3 letters, and start the next group at i + 1. There are at most
 * n / 3 large groups. */
int **largeGroupPositions(char *s, int *returnSize, int **returnColumnSizes)
{
    int n = (int)strlen(s), cap = n / 3 + 1, m = 0, start = 0, i;
    int **out = malloc((size_t)cap * sizeof *out);

    *returnColumnSizes = malloc((size_t)cap * sizeof **returnColumnSizes);
    for (i = 0; i < n; i++) {
        if (i + 1 < n && s[i + 1] == s[i])
            continue;
        if (i - start + 1 >= 3) {
            out[m] = malloc(2 * sizeof **out);
            out[m][0] = start;
            out[m][1] = i;
            (*returnColumnSizes)[m] = 2;
            m++;
        }
        start = i + 1;
    }
    *returnSize = m;
    return out;
}
