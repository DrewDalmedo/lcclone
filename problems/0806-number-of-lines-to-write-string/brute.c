/* Slow and obviously right: fill one row at a time. For the row that
 * starts at letter i, try ending it at every later letter, from the last
 * one down, adding up the widths afresh, and take the first (longest)
 * row whose total is at most 100. */
int *numberOfLines(int *widths, int widthsSize, char *s, int *returnSize)
{
    int n = (int)strlen(s), rows = 0, last = 0, i = 0, end, k, total = 0;
    int *out = malloc(2 * sizeof *out);

    (void)widthsSize;
    while (i < n) {
        for (end = n; end > i; end--) {
            total = 0;
            for (k = i; k < end; k++)
                total += widths[s[k] - 'a'];
            if (total <= 100)
                break;
        }
        rows++;
        last = total;
        i = end;
    }
    out[0] = rows;
    out[1] = last;
    *returnSize = 2;
    return out;
}
