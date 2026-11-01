/* Slow and obviously right: do the steps as the statement describes them.
 * Two words are anagrams when sorted copies of them are equal. Each step
 * strikes the last word that can be struck (a different order from the
 * one-pass solution, which keeps the first of each run), and the list is
 * scanned again from scratch after every step. */
static void sort_letters(char *s)
{
    int n = (int)strlen(s), i, j;
    char c;

    for (i = 1; i < n; i++) {
        c = s[i];
        for (j = i; j > 0 && s[j - 1] > c; j--)
            s[j] = s[j - 1];
        s[j] = c;
    }
}

static int anagrams(const char *a, const char *b)
{
    char x[16], y[16];

    if (strlen(a) != strlen(b))
        return 0;
    strcpy(x, a);
    strcpy(y, b);
    sort_letters(x);
    sort_letters(y);
    return strcmp(x, y) == 0;
}

char **removeAnagrams(char **words, int wordsSize, int *returnSize)
{
    char **list = malloc((size_t)wordsSize * sizeof *list), **out;
    int n = wordsSize, i, pick;

    memcpy(list, words, (size_t)wordsSize * sizeof *list);
    for (;;) {
        pick = -1;
        for (i = 1; i < n; i++)
            if (anagrams(list[i - 1], list[i]))
                pick = i;
        if (pick < 0)
            break;
        for (i = pick; i + 1 < n; i++)
            list[i] = list[i + 1];
        n--;
    }
    out = malloc((size_t)n * sizeof *out);
    for (i = 0; i < n; i++) {
        out[i] = malloc(strlen(list[i]) + 1);
        strcpy(out[i], list[i]);
    }
    free(list);
    *returnSize = n;
    return out;
}
