/* Reference solution: build each address's mailbox in one pass (in the
 * user part, skip dots and stop copying at the first '+'; copy the '@' and
 * the domain as they are), and count the mailboxes not yet in a hash set
 * (uthash). */
struct mailbox {
    char *key;
    UT_hash_handle hh;
};

int numUniqueEmails(char **emails, int emailsSize)
{
    struct mailbox *set = NULL, *items, *found;
    char **keys = malloc((size_t)emailsSize * sizeof *keys);
    int count = 0, i;

    items = malloc((size_t)emailsSize * sizeof *items);
    for (i = 0; i < emailsSize; i++) {
        const char *s = emails[i];
        char *m = malloc(strlen(s) + 1);
        size_t len = 0;
        int cut = 0;

        keys[i] = m;
        for (; *s != '@'; s++) {
            if (*s == '+')
                cut = 1;
            else if (*s != '.' && !cut)
                m[len++] = *s;
        }
        strcpy(m + len, s);
        HASH_FIND_STR(set, m, found);
        if (!found) {
            items[count].key = m;
            HASH_ADD_KEYPTR(hh, set, m, strlen(m), &items[count]);
            count++;
        }
    }
    HASH_CLEAR(hh, set);
    free(items);
    for (i = 0; i < emailsSize; i++)
        free(keys[i]);
    free(keys);
    return count;
}
