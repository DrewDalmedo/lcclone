/* Reference solution: walk the letters of pattern and the words of s
 * together, without changing s. Each letter keeps the word it lined up
 * with first: when the letter comes back, its word must be that one again,
 * and a letter seen for the first time must bring a word that none of the
 * letters seen so far (at most 25 others) has. In the end both must run
 * out together. */
static bool same_word(const char *a, size_t alen, const char *b, size_t blen)
{
    return alen == blen && memcmp(a, b, alen) == 0;
}

bool wordPattern(char *pattern, char *s)
{
    const char *word[26] = {NULL};
    size_t len[26];
    int seen[26], nseen = 0, i, j;
    const char *p = s;

    for (i = 0; pattern[i] != '\0'; i++) {
        int c = pattern[i] - 'a';
        const char *w = p;
        size_t wlen;

        if (*p == '\0')
            return false; /* fewer words than letters */
        while (*p != '\0' && *p != ' ')
            p++;
        wlen = (size_t)(p - w);
        if (*p == ' ')
            p++;
        if (word[c] != NULL) {
            if (!same_word(word[c], len[c], w, wlen))
                return false;
        } else {
            for (j = 0; j < nseen; j++)
                if (same_word(word[seen[j]], len[seen[j]], w, wlen))
                    return false;
            word[c] = w;
            len[c] = wlen;
            seen[nseen++] = c;
        }
    }
    return *p == '\0'; /* no words left over */
}
