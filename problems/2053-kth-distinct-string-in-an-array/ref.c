/* Reference solution: count how often each string occurs with a hash
 * table (uthash), then walk arr in order and return the k-th entry whose
 * count is 1, or "" if there are fewer. */
struct entry {
    char *key;
    int count;
    UT_hash_handle hh;
};

char *kthDistinct(char **arr, int arrSize, int k)
{
    struct entry *table = NULL, *items, *e;
    char *out = NULL;
    size_t len;
    int used = 0, i;

    items = malloc((size_t)arrSize * sizeof *items);
    for (i = 0; i < arrSize; i++) {
        HASH_FIND_STR(table, arr[i], e);
        if (e) {
            e->count++;
        } else {
            e = &items[used++];
            e->key = arr[i];
            e->count = 1;
            HASH_ADD_KEYPTR(hh, table, e->key, strlen(e->key), e);
        }
    }
    for (i = 0; i < arrSize && !out; i++) {
        HASH_FIND_STR(table, arr[i], e);
        if (e->count == 1 && --k == 0) {
            len = strlen(arr[i]) + 1;
            out = malloc(len);
            memcpy(out, arr[i], len);
        }
    }
    if (!out) {
        out = malloc(1);
        out[0] = '\0';
    }
    HASH_CLEAR(hh, table);
    free(items);
    return out;
}
