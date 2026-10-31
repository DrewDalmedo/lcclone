/* Reference solution: a hash table (uthash) from each string of list1 to
 * its index, then one walk along list2 that looks each string up and keeps
 * those with the smallest index sum so far. The walk stops once j alone is
 * more than the best sum, since i + j >= j. */
struct entry {
    const char *key;
    int index;
    UT_hash_handle hh;
};

char **findRestaurant(char **list1, int list1Size, char **list2,
                      int list2Size, int *returnSize)
{
    struct entry *table = NULL, *items, *e;
    int most = list1Size < list2Size ? list1Size : list2Size;
    char **out = malloc((size_t)most * sizeof *out);
    int best = INT_MAX, n = 0, i, j;

    items = malloc((size_t)list1Size * sizeof *items);
    for (i = 0; i < list1Size; i++) {
        items[i].key = list1[i];
        items[i].index = i;
        HASH_ADD_KEYPTR(hh, table, items[i].key, strlen(items[i].key),
                        &items[i]);
    }
    for (j = 0; j < list2Size && j <= best; j++) {
        int sum;

        HASH_FIND_STR(table, list2[j], e);
        if (!e)
            continue;
        sum = e->index + j;
        if (sum < best) {
            best = sum;
            n = 0;
        }
        if (sum == best)
            out[n++] = list2[j];
    }
    for (i = 0; i < n; i++) {
        size_t len = strlen(out[i]) + 1;
        char *copy = malloc(len);

        memcpy(copy, out[i], len);
        out[i] = copy;
    }
    HASH_CLEAR(hh, table);
    free(items);
    *returnSize = n;
    return out;
}
