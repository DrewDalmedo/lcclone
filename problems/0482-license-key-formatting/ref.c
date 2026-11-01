/* Reference solution: count the letters and digits, m, which fixes the
 * length of the answer at m + (m - 1) / k. Then walk s backwards and fill
 * the answer from its right end, putting a dash in front of every k
 * characters written except at the very start. */
char *licenseKeyFormatting(char *s, int k)
{
    size_t n = strlen(s), m = 0, len, written = 0, i, j;
    char *out;
    char c;

    for (i = 0; i < n; i++)
        if (s[i] != '-')
            m++;
    len = m == 0 ? 0 : m + (m - 1) / (size_t)k;
    out = malloc(len + 1);
    out[len] = '\0';
    j = len;
    for (i = n; i-- > 0;) {
        c = s[i];
        if (c == '-')
            continue;
        if (written > 0 && written % (size_t)k == 0)
            out[--j] = '-';
        out[--j] = c >= 'a' && c <= 'z' ? (char)(c - 'a' + 'A') : c;
        written++;
    }
    return out;
}
