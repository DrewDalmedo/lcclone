/* Reference solution: write each word's signal into a buffer of 49 bytes
 * (12 letters of at most 4 symbols each, and the NUL), and keep the
 * different signals in a hash table (uthash). The answer is the number of
 * signals in the table. */
static const char *const morse[26] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."
};

struct signal {
    char text[49];
    UT_hash_handle hh;
};

int uniqueMorseRepresentations(char **words, int wordsSize)
{
    struct signal *items = malloc((size_t)wordsSize * sizeof *items);
    struct signal *table = NULL, *s, *found;
    int distinct = 0, i;
    const char *p;
    size_t len, n;

    for (i = 0; i < wordsSize; i++) {
        s = &items[distinct];
        len = 0;
        for (p = words[i]; *p; p++) {
            n = strlen(morse[*p - 'a']);
            memcpy(s->text + len, morse[*p - 'a'], n);
            len += n;
        }
        s->text[len] = '\0';
        HASH_FIND_STR(table, s->text, found);
        if (!found) {
            HASH_ADD_STR(table, text, s);
            distinct++;
        }
    }
    HASH_CLEAR(hh, table);
    free(items);
    return distinct;
}
