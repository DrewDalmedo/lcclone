/* Reference solution: one pass, growing the current block while the
 * character stays the same and keeping the longest length seen for each
 * of the two characters. */
bool checkZeroOnes(char *s)
{
    int best[2] = {0, 0}, len = 0, i;

    for (i = 0; s[i] != '\0'; i++) {
        len = (i > 0 && s[i] == s[i - 1]) ? len + 1 : 1;
        if (len > best[s[i] - '0'])
            best[s[i] - '0'] = len;
    }
    return best[1] > best[0];
}
