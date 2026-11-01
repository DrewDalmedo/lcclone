/* Slow and obviously right: split a copy of s into its words, compare the
 * counts, then check every pair of positions: the letters must be equal
 * exactly when the words are. */
bool wordPattern(char *pattern, char *s)
{
    size_t m = strlen(pattern), k = 0, i, j;
    char *copy = malloc(strlen(s) + 1), *tok;
    char **words = malloc((strlen(s) / 2 + 1) * sizeof *words);
    bool ok = true;

    strcpy(copy, s);
    for (tok = strtok(copy, " "); tok != NULL; tok = strtok(NULL, " "))
        words[k++] = tok;
    if (k != m)
        ok = false;
    for (i = 0; ok && i < m; i++)
        for (j = i + 1; ok && j < m; j++)
            if ((pattern[i] == pattern[j]) != (strcmp(words[i], words[j]) == 0))
                ok = false;
    free(words);
    free(copy);
    return ok;
}
