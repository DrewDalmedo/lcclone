/* Reference solution: put every city that a leg sets out from in a hash
 * set (uthash), then return the one destination of a leg that is not in
 * the set. */
struct city {
    const char *name;
    UT_hash_handle hh;
};

char *destCity(char ***paths, int pathsSize, int *pathsColSize)
{
    struct city *froms = malloc((size_t)pathsSize * sizeof *froms);
    struct city *set = NULL, *found;
    const char *end = NULL;
    char *out;
    size_t len;
    int i;

    (void)pathsColSize;
    for (i = 0; i < pathsSize; i++) {
        froms[i].name = paths[i][0];
        HASH_ADD_KEYPTR(hh, set, froms[i].name, strlen(froms[i].name),
                        &froms[i]);
    }
    for (i = 0; i < pathsSize && !end; i++) {
        HASH_FIND_STR(set, paths[i][1], found);
        if (!found)
            end = paths[i][1];
    }
    HASH_CLEAR(hh, set);
    free(froms);
    len = strlen(end) + 1;
    out = malloc(len);
    memcpy(out, end, len);
    return out;
}
