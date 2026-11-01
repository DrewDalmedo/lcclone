/* Slow and obviously right: at each position try both kinds of piece (a
 * digit 1 to 9, or two digits making 10 to 26 followed by '#'), and
 * backtrack until a choice of pieces uses up the whole string. */
static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static int decode(const char *s, size_t i, char *out, size_t len)
{
    int v;

    if (s[i] == '\0') {
        out[len] = '\0';
        return 1;
    }
    if (s[i] >= '1' && s[i] <= '9') {
        out[len] = (char)('a' + (s[i] - '1'));
        if (decode(s, i + 1, out, len + 1))
            return 1;
    }
    if (is_digit(s[i]) && is_digit(s[i + 1]) && s[i + 2] == '#') {
        v = (s[i] - '0') * 10 + (s[i + 1] - '0');
        if (v >= 10 && v <= 26) {
            out[len] = (char)('a' + v - 1);
            if (decode(s, i + 3, out, len + 1))
                return 1;
        }
    }
    return 0;
}

char *freqAlphabets(char *s)
{
    char *out = malloc(strlen(s) + 1);

    out[0] = '\0';
    decode(s, 0, out, 0);
    return out;
}
