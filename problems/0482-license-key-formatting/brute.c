/* Slow and obviously right, straight from the definition: make t, the key
 * without dashes and in capitals. Then, while t is not empty, cut off its
 * last k characters (or all of them, when fewer are left) and put them in
 * front of the result, followed by a dash if the result already holds a
 * block. */
char *licenseKeyFormatting(char *s, int k)
{
    size_t n = strlen(s), m = 0, len = 0, take, i;
    char *t = malloc(n + 1), *out = malloc(1), *next;

    for (i = 0; i < n; i++)
        if (s[i] != '-')
            t[m++] = (char)toupper((unsigned char)s[i]);
    out[0] = '\0';
    while (m > 0) {
        take = m < (size_t)k ? m : (size_t)k;
        next = malloc(take + 1 + len + 1);
        memcpy(next, t + m - take, take);
        if (len > 0) {
            next[take] = '-';
            memcpy(next + take + 1, out, len + 1);
            len += take + 1;
        } else {
            next[take] = '\0';
            len = take;
        }
        free(out);
        out = next;
        m -= take;
    }
    free(t);
    return out;
}
