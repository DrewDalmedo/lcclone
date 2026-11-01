/* Reference solution: walk along s counting spaces; the preview ends just
 * before the k-th space, or at the end of s if there are fewer. */
char *truncateSentence(char *s, int k)
{
    size_t len = 0;
    int spaces = 0;
    char *out;

    while (s[len] && !(s[len] == ' ' && ++spaces == k))
        len++;
    out = malloc(len + 1);
    memcpy(out, s, len);
    out[len] = '\0';
    return out;
}
