/* Reference solution: a hash table (uthash) of the words of words1 keeps
 * two counts per word, one for each list, capped at 2; the answer is the
 * number of words whose counts are both 1. */
struct entry {
    const char *key;
    int count[2];
    UT_hash_handle hh;
};

int countWords(char **words1, int words1Size, char **words2, int words2Size)
{
    struct entry *table = NULL, *items, *e;
    int used = 0, paired = 0, i;

    items = malloc((size_t)words1Size * sizeof *items);
    for (i = 0; i < words1Size; i++) {
        HASH_FIND_STR(table, words1[i], e);
        if (!e) {
            e = &items[used++];
            e->key = words1[i];
            e->count[0] = e->count[1] = 0;
            HASH_ADD_KEYPTR(hh, table, e->key, strlen(e->key), e);
        }
        if (e->count[0] < 2)
            e->count[0]++;
    }
    for (i = 0; i < words2Size; i++) {
        HASH_FIND_STR(table, words2[i], e);
        if (e && e->count[1] < 2)
            e->count[1]++;
    }
    for (i = 0; i < used; i++)
        if (items[i].count[0] == 1 && items[i].count[1] == 1)
            paired++;
    HASH_CLEAR(hh, table);
    free(items);
    return paired;
}
