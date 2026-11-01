/* Slow and obviously right: with c ones in s, a single run of 1s starting
 * at s[0] means s is c ones and then nothing but zeros. Build that string
 * and compare. */
bool checkOnesSegment(char *s)
{
    int n = (int)strlen(s), c = 0, i;
    char *shape = malloc((size_t)n + 1);
    bool same;

    for (i = 0; i < n; i++)
        if (s[i] == '1')
            c++;
    for (i = 0; i < n; i++)
        shape[i] = i < c ? '1' : '0';
    shape[n] = '\0';
    same = strcmp(shape, s) == 0;
    free(shape);
    return same;
}
