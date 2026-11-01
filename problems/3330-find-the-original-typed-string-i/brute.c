/* Slow and obviously right: list every string that word can come from,
 * namely word itself and, for each position i and each e >= 1 such that
 * the e letters after word[i] are all copies of it, word with those e
 * letters removed. Then sort the list and count the different strings. */
static int cmp(const void *a, const void *b)
{
    return strcmp(*(char *const *)a, *(char *const *)b);
}

int possibleStringCount(char *word)
{
    int n = (int)strlen(word), m = 0, distinct, i, e;
    char **list = malloc((size_t)(n * n + 1) * sizeof *list);

    list[m] = malloc((size_t)n + 1);
    memcpy(list[m++], word, (size_t)n + 1);
    for (i = 0; i < n; i++)
        for (e = 1; i + e < n && word[i + e] == word[i]; e++) {
            list[m] = malloc((size_t)(n - e) + 1);
            memcpy(list[m], word, (size_t)i + 1);
            memcpy(list[m] + i + 1, word + i + 1 + e, (size_t)(n - i - e));
            m++;
        }
    qsort(list, (size_t)m, sizeof *list, cmp);
    distinct = 0;
    for (i = 0; i < m; i++)
        if (i == 0 || strcmp(list[i], list[i - 1]) != 0)
            distinct++;
    for (i = 0; i < m; i++)
        free(list[i]);
    free(list);
    return distinct;
}
