/* Slow and obviously right: work out each index on its own. A space is
 * copied; any other character is in a word that runs from a to b (found
 * by walking out to the spaces on each side), and index i of the result
 * gets the character at the mirror position a + b - i. */
char *reverseWords(char *s)
{
    long n = (long)strlen(s), i, a, b;
    char *out = malloc((size_t)n + 1);

    for (i = 0; i < n; i++) {
        if (s[i] == ' ') {
            out[i] = ' ';
            continue;
        }
        a = i;
        while (a > 0 && s[a - 1] != ' ')
            a--;
        b = i;
        while (b + 1 < n && s[b + 1] != ' ')
            b++;
        out[i] = s[a + b - i];
    }
    out[n] = '\0';
    return out;
}
