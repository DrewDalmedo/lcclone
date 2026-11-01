/* Reference solution: two nested loops, columns outside and rows inside,
 * each cell written as its letter, its digit and a NUL. */
char **cellsInRange(char *s, int *returnSize)
{
    char c1 = s[0], r1 = s[1], c2 = s[3], r2 = s[4], c, r;
    int total = (c2 - c1 + 1) * (r2 - r1 + 1), m = 0;
    char **out = malloc((size_t)total * sizeof *out);

    for (c = c1; c <= c2; c++)
        for (r = r1; r <= r2; r++) {
            out[m] = malloc(3);
            out[m][0] = c;
            out[m][1] = r;
            out[m][2] = '\0';
            m++;
        }
    *returnSize = m;
    return out;
}
