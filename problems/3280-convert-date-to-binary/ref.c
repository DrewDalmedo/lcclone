/* Reference solution: read each decimal field, then write it in binary by
 * taking off its lowest bit again and again and reversing those bits. */
static int field(const char *s, int len)
{
    int v = 0, i;

    for (i = 0; i < len; i++)
        v = v * 10 + (s[i] - '0');
    return v;
}

/* Writes v (at least 1) in binary at out and returns how many digits. */
static int write_binary(char *out, int v)
{
    char low_first[16];
    int n = 0, i;

    while (v > 0) {
        low_first[n++] = (char)('0' + (v & 1));
        v >>= 1;
    }
    for (i = 0; i < n; i++)
        out[i] = low_first[n - 1 - i];
    return n;
}

char *convertDateToBinary(char *date)
{
    /* At most 12 + 1 + 4 + 1 + 5 characters, and the NUL. */
    char *out = malloc(24);
    int len;

    len = write_binary(out, field(date, 4));
    out[len++] = '-';
    len += write_binary(out + len, field(date + 5, 2));
    out[len++] = '-';
    len += write_binary(out + len, field(date + 8, 2));
    out[len] = '\0';
    return out;
}
