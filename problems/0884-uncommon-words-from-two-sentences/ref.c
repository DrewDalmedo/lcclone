/* Reference solution: split copies of both sentences into words, count
 * each word in a hash table (uthash), and return the words counted exactly
 * once, in the order they first appear. */
struct word {
    char *key;
    int count;
    UT_hash_handle hh;
};

/* Cuts s into its words in place and stores where each starts. */
static int split(char *s, char **words)
{
    int n = 0;

    for (;;) {
        words[n++] = s;
        while (*s && *s != ' ')
            s++;
        if (!*s)
            return n;
        *s++ = '\0';
    }
}

char **uncommonFromSentences(char *s1, char *s2, int *returnSize)
{
    size_t len1 = strlen(s1), len2 = strlen(s2);
    char *buf = malloc(len1 + len2 + 2);
    char **words = malloc((len1 + len2 + 2) * sizeof *words);
    struct word *table = NULL, *items, *w;
    char **out;
    int n, distinct = 0, m = 0, i;

    memcpy(buf, s1, len1 + 1);
    memcpy(buf + len1 + 1, s2, len2 + 1);
    n = split(buf, words);
    n += split(buf + len1 + 1, words + n);
    items = malloc((size_t)n * sizeof *items);
    for (i = 0; i < n; i++) {
        HASH_FIND_STR(table, words[i], w);
        if (w) {
            w->count++;
        } else {
            w = &items[distinct++];
            w->key = words[i];
            w->count = 1;
            HASH_ADD_KEYPTR(hh, table, w->key, strlen(w->key), w);
        }
    }
    out = malloc((size_t)distinct * sizeof *out);
    for (i = 0; i < distinct; i++)
        if (items[i].count == 1) {
            size_t len = strlen(items[i].key) + 1;

            out[m] = malloc(len);
            memcpy(out[m], items[i].key, len);
            m++;
        }
    HASH_CLEAR(hh, table);
    free(items);
    free(words);
    free(buf);
    *returnSize = m;
    return out;
}
