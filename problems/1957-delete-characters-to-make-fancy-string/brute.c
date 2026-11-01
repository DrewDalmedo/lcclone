/* Slow and obviously right: try every way of keeping some of the
 * characters of s (2^n of them, so small inputs only), and among the ones
 * that leave a fancy string, take one that keeps the most. Along the way,
 * check the claim that all of those leave the same string. */
static int fancy(const char *t, int len)
{
    int i;

    for (i = 2; i < len; i++)
        if (t[i] == t[i - 1] && t[i] == t[i - 2])
            return 0;
    return 1;
}

char *makeFancyString(char *s)
{
    int n = (int)strlen(s), best = -1, differ = 0, len, i;
    char *t = malloc((size_t)n + 1), *out = malloc((size_t)n + 1);
    unsigned long mask;

    for (mask = 0; mask < (1UL << n); mask++) {
        len = 0;
        for (i = 0; i < n; i++)
            if (mask >> i & 1)
                t[len++] = s[i];
        t[len] = '\0';
        if (len < best || !fancy(t, len))
            continue;
        if (len > best) {
            best = len;
            memcpy(out, t, (size_t)len + 1);
            differ = 0;
        } else if (strcmp(out, t) != 0) {
            differ = 1;
        }
    }
    if (differ)
        abort(); /* two different answers with the fewest strikes */
    free(t);
    return out;
}
