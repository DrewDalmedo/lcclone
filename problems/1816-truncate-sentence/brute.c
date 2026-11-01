/* Slow and obviously right: split a copy of s into its words, then build
 * the answer by appending the first k of them, with a space before every
 * word but the first. */
char *truncateSentence(char *s, int k)
{
    char *copy = malloc(strlen(s) + 1), *words[250], *tok;
    char *out = malloc(strlen(s) + 1);
    int n = 0, i;

    strcpy(copy, s);
    for (tok = strtok(copy, " "); tok; tok = strtok(NULL, " "))
        words[n++] = tok;
    out[0] = '\0';
    for (i = 0; i < k && i < n; i++) {
        if (i > 0)
            strcat(out, " ");
        strcat(out, words[i]);
    }
    free(copy);
    return out;
}
