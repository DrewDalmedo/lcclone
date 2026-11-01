/* Slow and obviously right: for each position, walk k single steps to the
 * right through s, jumping back to the start after the last letter. */
char *getEncryptedString(char *s, int k)
{
    int n = (int)strlen(s), i, step, at;
    char *t = malloc((size_t)n + 1);

    for (i = 0; i < n; i++) {
        at = i;
        for (step = 0; step < k; step++) {
            at++;
            if (at == n)
                at = 0;
        }
        t[i] = s[at];
    }
    t[n] = '\0';
    return t;
}
