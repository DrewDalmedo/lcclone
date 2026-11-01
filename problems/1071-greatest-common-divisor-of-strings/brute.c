/* Slow and obviously right: a stamp that prints str1 is a prefix of it.
 * Try every prefix of str1, longest first, and keep the first one whose
 * repeats spell out both strings exactly. */
static int prints(const char *x, size_t len, const char *s)
{
    size_t n = strlen(s), i;

    if (n % len != 0)
        return 0;
    for (i = 0; i < n; i++)
        if (s[i] != x[i % len])
            return 0;
    return 1;
}

char *gcdOfStrings(char *str1, char *str2)
{
    size_t len = strlen(str1);
    char *out;

    while (len > 0 && !(prints(str1, len, str1) && prints(str1, len, str2)))
        len--;
    out = malloc(len + 1);
    memcpy(out, str1, len);
    out[len] = '\0';
    return out;
}
