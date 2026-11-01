/* Reference solution: copy paragraph in lower case with a NUL in place of
 * every non-letter, so that each word ends in a NUL, and count the words
 * in a hash table (uthash). The banned words go in a second table; the
 * answer is the counted word with the highest count that isn't in it. */
struct entry {
    char *key;
    int count;
    UT_hash_handle hh;
};

static int is_letter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

char *mostCommonWord(char *paragraph, char **banned, int bannedSize)
{
    size_t n = strlen(paragraph), i, len;
    char *text = malloc(n + 1), *out;
    struct entry *words = malloc((n / 2 + 1) * sizeof *words);
    struct entry *bans = malloc(((size_t)bannedSize + 1) * sizeof *bans);
    struct entry *counts = NULL, *banset = NULL, *e, *best = NULL;
    int nwords = 0, nbans = 0, k;

    for (i = 0; i < n; i++) {
        char c = paragraph[i];

        if (c >= 'A' && c <= 'Z')
            c = (char)(c - 'A' + 'a');
        text[i] = is_letter(c) ? c : '\0';
    }
    text[n] = '\0';
    for (k = 0; k < bannedSize; k++) {
        HASH_FIND_STR(banset, banned[k], e);
        if (!e) {
            e = &bans[nbans++];
            e->key = banned[k];
            HASH_ADD_KEYPTR(hh, banset, e->key, strlen(e->key), e);
        }
    }
    for (i = 0; i < n; i++) {
        if (text[i] == '\0' || (i > 0 && text[i - 1] != '\0'))
            continue;
        HASH_FIND_STR(counts, text + i, e);
        if (e) {
            e->count++;
        } else {
            e = &words[nwords++];
            e->key = text + i;
            e->count = 1;
            HASH_ADD_KEYPTR(hh, counts, e->key, strlen(e->key), e);
        }
    }
    for (k = 0; k < nwords; k++) {
        HASH_FIND_STR(banset, words[k].key, e);
        if (!e && (!best || words[k].count > best->count))
            best = &words[k];
    }
    len = strlen(best->key);
    out = malloc(len + 1);
    memcpy(out, best->key, len + 1);
    HASH_CLEAR(hh, counts);
    HASH_CLEAR(hh, banset);
    free(bans);
    free(words);
    free(text);
    return out;
}
