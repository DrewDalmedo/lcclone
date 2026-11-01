/* Slow and obviously right: type the keys one by one into a buffer, and at
 * each 'i' reverse everything typed so far, swapping from both ends. */
char *finalString(char *s)
{
    size_t n = strlen(s), len = 0, a, b, i;
    char *out = malloc(n + 1), t;

    for (i = 0; i < n; i++) {
        if (s[i] != 'i') {
            out[len++] = s[i];
            continue;
        }
        for (a = 0, b = len; a + 1 < b; a++, b--) {
            t = out[a];
            out[a] = out[b - 1];
            out[b - 1] = t;
        }
    }
    out[len] = '\0';
    return out;
}
