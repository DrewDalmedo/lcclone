/* Random words over the first few letters of the alphabet, so that many
 * letters are common, with repeats. Small cases use a, b and c (sometimes
 * d); full-size ones have 100 words of 100 letters. */
void gen(int small)
{
    int n = (int)(small ? rnd(1, 5) : 100), len, i, k;
    int letters = (int)(small ? rnd(3, 4) : rnd(1, 26));
    char **words = malloc((size_t)n * sizeof *words);

    for (i = 0; i < n; i++) {
        len = (int)(small ? rnd(1, 6) : 100);
        words[i] = malloc((size_t)len + 1);
        for (k = 0; k < len; k++)
            words[i][k] = (char)('a' + rnd(0, letters - 1));
        words[i][len] = '\0';
    }
    put_strings(words, n);
    for (i = 0; i < n; i++)
        free(words[i]);
    free(words);
}
