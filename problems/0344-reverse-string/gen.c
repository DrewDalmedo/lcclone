/* Random printable ASCII characters. A small case draws from "ab", from a
 * few characters with a space, a quote and a backslash among them, or from
 * all of 32..126, so that repeats and palindromes come up often; a
 * full-size case draws from all of 32..126. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100000, i;
    int kind = small ? (int)rnd(0, 2) : 2;
    char *s = malloc((size_t)n);

    for (i = 0; i < n; i++)
        if (kind == 0)
            s[i] = rnd_char("ab");
        else if (kind == 1)
            s[i] = rnd_char("aZ7 \"\\");
        else
            s[i] = (char)rnd(32, 126);
    put_chars(s, n);
    free(s);
}
