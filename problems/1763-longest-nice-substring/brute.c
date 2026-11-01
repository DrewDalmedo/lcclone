/* Slow and obviously right: try the lengths from longest to shortest and,
 * for each, the starts from left to right; check a substring by looking up
 * the other case of each of its letters inside it. The first nice one
 * found is the answer. */
static int is_nice(const char *s, int from, int len)
{
    int i, j, found;
    char other;

    for (i = from; i < from + len; i++) {
        other = islower((unsigned char)s[i]) ? (char)toupper((unsigned char)s[i])
                                             : (char)tolower((unsigned char)s[i]);
        found = 0;
        for (j = from; j < from + len; j++)
            if (s[j] == other)
                found = 1;
        if (!found)
            return 0;
    }
    return 1;
}

char *longestNiceSubstring(char *s)
{
    int n = (int)strlen(s), len, from;
    char *out;

    for (len = n; len >= 1; len--)
        for (from = 0; from + len <= n; from++)
            if (is_nice(s, from, len)) {
                out = malloc((size_t)len + 1);
                memcpy(out, s + from, (size_t)len);
                out[len] = '\0';
                return out;
            }
    out = malloc(1);
    out[0] = '\0';
    return out;
}
