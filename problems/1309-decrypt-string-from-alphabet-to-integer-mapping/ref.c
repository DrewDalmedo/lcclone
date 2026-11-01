/* Reference solution: scan s from the left. When the character two places
 * ahead is a '#', the two digits here make one letter; otherwise this
 * digit alone does. */
char *freqAlphabets(char *s)
{
    size_t n = strlen(s), i = 0, len = 0;
    char *out = malloc(n + 1);

    while (i < n) {
        if (i + 2 < n && s[i + 2] == '#') {
            out[len++] = (char)('a' + (s[i] - '0') * 10 + (s[i + 1] - '0') - 1);
            i += 3;
        } else {
            out[len++] = (char)('a' + (s[i] - '1'));
            i++;
        }
    }
    out[len] = '\0';
    return out;
}
