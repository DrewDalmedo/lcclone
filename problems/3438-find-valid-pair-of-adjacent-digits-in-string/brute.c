/* Slow and obviously right: for each pair from the left, count the copies
 * of both its digits over the whole string anew. */
static int copies(const char *s, char c)
{
    int n = 0;

    for (; *s; s++)
        if (*s == c)
            n++;
    return n;
}

char *findValidPair(char *s)
{
    int n = (int)strlen(s), i;
    char *out = malloc(3);

    out[0] = '\0';
    for (i = 0; i + 1 < n; i++)
        if (s[i] != s[i + 1] && copies(s, s[i]) == s[i] - '0'
            && copies(s, s[i + 1]) == s[i + 1] - '0') {
            out[0] = s[i];
            out[1] = s[i + 1];
            out[2] = '\0';
            break;
        }
    return out;
}
