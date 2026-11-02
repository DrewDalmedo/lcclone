/* Reference solution: count every digit of s once, then scan the
 * neighbouring pairs from the left and return the first whose two digits
 * differ and each occur as many times as their value. */
char *findValidPair(char *s)
{
    int count[10] = {0};
    char *out = malloc(3);
    int i;

    for (i = 0; s[i]; i++)
        count[s[i] - '0']++;
    out[0] = '\0';
    for (i = 0; s[i] && s[i + 1]; i++) {
        int a = s[i] - '0', b = s[i + 1] - '0';

        if (a != b && count[a] == a && count[b] == b) {
            out[0] = s[i];
            out[1] = s[i + 1];
            out[2] = '\0';
            break;
        }
    }
    return out;
}
