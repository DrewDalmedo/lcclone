/* Slow and obviously right: deal the letters one at a time into the
 * current row, start a new row whenever one fills up, and pad the last
 * row with fill if it was left part-way. */
char **divideString(char *s, int k, char fill, int *returnSize)
{
    char **out = malloc(200 * sizeof *out);
    int rows = 0, used = 0, i;

    for (i = 0; s[i]; i++) {
        if (used == 0) {
            out[rows] = calloc((size_t)k + 1, 1);
            rows++;
        }
        out[rows - 1][used] = s[i];
        used++;
        if (used == k)
            used = 0;
    }
    if (used > 0)
        while (used < k)
            out[rows - 1][used++] = fill;
    *returnSize = rows;
    return out;
}
