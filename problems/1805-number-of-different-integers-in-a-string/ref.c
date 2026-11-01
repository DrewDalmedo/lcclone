/* Reference solution: walk word once, cutting out each number and dropping
 * its leading zeros (all but the last digit of a number made only of
 * zeros), and keep the trimmed digit strings in a hash table (uthash),
 * keyed by their characters. The answer is the number of distinct keys.
 * Comparing digit strings instead of parsed values works for any length. */
struct number {
    const char *digits;
    UT_hash_handle hh;
};

int numDifferentIntegers(char *word)
{
    int n = (int)strlen(word), count = 0, i = 0, start;
    struct number *items = malloc(((size_t)n / 2 + 1) * sizeof *items);
    struct number *table = NULL, *found;

    while (i < n) {
        if (word[i] < '0' || word[i] > '9') {
            i++;
            continue;
        }
        start = i;
        while (i < n && word[i] >= '0' && word[i] <= '9')
            i++;
        while (start < i - 1 && word[start] == '0')
            start++;
        HASH_FIND(hh, table, word + start, (unsigned)(i - start), found);
        if (!found) {
            items[count].digits = word + start;
            HASH_ADD_KEYPTR(hh, table, items[count].digits,
                            (unsigned)(i - start), &items[count]);
            count++;
        }
    }
    HASH_CLEAR(hh, table);
    free(items);
    return count;
}
