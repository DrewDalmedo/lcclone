/* Slow and obviously right: list the words of both sentences, and for each
 * one count how many words of the list are equal to it, keeping the words
 * whose count is 1. */
char **uncommonFromSentences(char *s1, char *s2, int *returnSize)
{
    char *words[202], *copy[2], *tok;
    char **out = malloc(202 * sizeof *out);
    int n = 0, m = 0, i, k, count, which;

    copy[0] = malloc(strlen(s1) + 1);
    strcpy(copy[0], s1);
    copy[1] = malloc(strlen(s2) + 1);
    strcpy(copy[1], s2);
    for (which = 0; which < 2; which++)
        for (tok = strtok(copy[which], " "); tok; tok = strtok(NULL, " "))
            words[n++] = tok;
    for (i = 0; i < n; i++) {
        count = 0;
        for (k = 0; k < n; k++)
            if (strcmp(words[i], words[k]) == 0)
                count++;
        if (count == 1) {
            out[m] = malloc(strlen(words[i]) + 1);
            strcpy(out[m], words[i]);
            m++;
        }
    }
    free(copy[0]);
    free(copy[1]);
    *returnSize = m;
    return out;
}
