/* Reference solution: only k mod n matters, so t is s with its first
 * (k mod n) letters moved to the end; copy the two parts into a new buffer. */
char *getEncryptedString(char *s, int k)
{
    size_t n = strlen(s), m = (size_t)k % n;
    char *t = malloc(n + 1);

    memcpy(t, s + m, n - m);
    memcpy(t + (n - m), s, m);
    t[n] = '\0';
    return t;
}
