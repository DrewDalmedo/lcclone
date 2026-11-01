/* Slow and obviously right: read the words of paragraph one letter at a
 * time into a list of lower-case strings. Then, for each word that is
 * equal to no banned entry, count the words of the list equal to it, and
 * keep the word with the largest count. */
char *mostCommonWord(char *paragraph, char **banned, int bannedSize)
{
    size_t n = strlen(paragraph), i, len = 0;
    char **list = malloc((n + 1) * sizeof *list), *cur = NULL, *out;
    int nlist = 0, best = -1, bestCount = 0, count, ban, k, j;

    for (i = 0; i <= n; i++) {
        char c = paragraph[i];

        if (c >= 'A' && c <= 'Z')
            c = (char)(c - 'A' + 'a');
        if (c >= 'a' && c <= 'z') {
            if (!cur) {
                cur = malloc(n + 1);
                len = 0;
            }
            cur[len++] = c;
        } else if (cur) {
            cur[len] = '\0';
            list[nlist++] = cur;
            cur = NULL;
        }
    }
    for (k = 0; k < nlist; k++) {
        ban = 0;
        for (j = 0; j < bannedSize; j++)
            if (strcmp(list[k], banned[j]) == 0)
                ban = 1;
        if (ban)
            continue;
        count = 0;
        for (j = 0; j < nlist; j++)
            if (strcmp(list[k], list[j]) == 0)
                count++;
        if (count > bestCount) {
            best = k;
            bestCount = count;
        }
    }
    out = malloc(strlen(list[best]) + 1);
    strcpy(out, list[best]);
    for (k = 0; k < nlist; k++)
        free(list[k]);
    free(list);
    return out;
}
