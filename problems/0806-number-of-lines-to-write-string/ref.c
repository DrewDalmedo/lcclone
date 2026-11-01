/* Reference solution: one pass over s, keeping the number of rows and the
 * width of the current row. A letter that would take the row past 100
 * starts a new row instead. */
int *numberOfLines(int *widths, int widthsSize, char *s, int *returnSize)
{
    int rows = 1, width = 0, w, i;
    int *out = malloc(2 * sizeof *out);

    (void)widthsSize;
    for (i = 0; s[i]; i++) {
        w = widths[s[i] - 'a'];
        if (width + w <= 100) {
            width += w;
        } else {
            rows++;
            width = w;
        }
    }
    out[0] = rows;
    out[1] = width;
    *returnSize = 2;
    return out;
}
