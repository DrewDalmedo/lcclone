/* Slow and obviously right: split a copy of the sentence into its words,
 * then check each word against the next, and the last against the
 * first. */
bool isCircularSentence(char *sentence)
{
    size_t m = strlen(sentence);
    char *copy = malloc(m + 1), **words = malloc((m + 1) * sizeof *words);
    char *tok;
    int k = 0, i, ok = 1;
    size_t len;

    strcpy(copy, sentence);
    for (tok = strtok(copy, " "); tok; tok = strtok(NULL, " "))
        words[k++] = tok;
    for (i = 0; i < k; i++) {
        /* word i and the word after it, the last wrapping to the first */
        const char *a = words[i], *b = words[(i + 1) % k];

        len = strlen(a);
        if (a[len - 1] != b[0])
            ok = 0;
    }
    free(words);
    free(copy);
    return ok;
}
