/* A random alphabet (a shuffle of the 26 letters) and a list of words.
 *
 * Small cases: 1 to 5 words of 1 to 4 letters from "abc" or "ab" (whose
 * ranks the shuffle decides), each new word random or made from an
 * earlier one (a copy, a prefix, or one letter longer). Half the time the
 * list is then sorted in the alphabet, and some of those get one pair of
 * neighbours swapped.
 *
 * Full-size cases: 100 words of 20 letters that share a prefix of 10 to
 * 19 letters, sorted in the alphabet. The seed picks the shape:
 *   0. left sorted (true);
 *   1. the last two different words swapped (false at the far end);
 *   2. the last word cut to a proper prefix of the word before it (false
 *      at the far end, by the prefix rule). */
static int rank_of[26];

static int cmp_alien(const char *a, const char *b)
{
    int k;

    for (k = 0; a[k] && a[k] == b[k]; k++)
        ;
    if (!a[k] || !b[k])
        return (a[k] != 0) - (b[k] != 0);
    return rank_of[a[k] - 'a'] - rank_of[b[k] - 'a'];
}

static void sort_alien(char **w, int n)
{
    char *t;
    int i, k;

    for (i = 1; i < n; i++)
        for (k = i; k > 0 && cmp_alien(w[k - 1], w[k]) > 0; k--) {
            t = w[k];
            w[k] = w[k - 1];
            w[k - 1] = t;
        }
}

static void swap_words(char **w, int i)
{
    char *t = w[i];

    w[i] = w[i + 1];
    w[i + 1] = t;
}

void gen(int small)
{
    static char store[100][21];
    char order[27], prefix[20], *words[100];
    int perm[26], n = small ? (int)rnd(1, 5) : 100;
    int shape = small ? -1 : (int)(gen_seed() % 3);
    int i, k, len, from, how, plen, pick;
    const char *letters = rnd_chance(70) ? "abc" : "ab";

    for (i = 0; i < 26; i++)
        perm[i] = i;
    rnd_shuffle(perm, 26);
    for (i = 0; i < 26; i++) {
        order[i] = (char)('a' + perm[i]);
        rank_of[perm[i]] = i;
    }
    order[26] = '\0';
    for (i = 0; i < n; i++)
        words[i] = store[i];
    if (small) {
        for (i = 0; i < n; i++) {
            how = i > 0 && rnd_chance(50) ? (int)rnd(1, 3) : 0;
            from = i > 0 ? (int)rnd(0, i - 1) : 0;
            len = (int)strlen(store[from]);
            if (how == 1 || (how == 2 && len == 1)) {
                strcpy(store[i], store[from]);
            } else if (how == 2) {
                plen = (int)rnd(1, len - 1);
                memcpy(store[i], store[from], (size_t)plen);
                store[i][plen] = '\0';
            } else if (how == 3 && len < 20) {
                strcpy(store[i], store[from]);
                store[i][len] = rnd_char(letters);
                store[i][len + 1] = '\0';
            } else {
                len = (int)rnd(1, 4);
                for (k = 0; k < len; k++)
                    store[i][k] = rnd_char(letters);
                store[i][len] = '\0';
            }
        }
        if (rnd_chance(50)) {
            sort_alien(words, n);
            if (n > 1 && rnd_chance(40)) {
                pick = (int)rnd(0, n - 2);
                swap_words(words, pick);
            }
        }
    } else {
        plen = (int)rnd(10, 19);
        for (k = 0; k < plen; k++)
            prefix[k] = rnd_char("abcdefghijklmnopqrstuvwxyz");
        for (i = 0; i < n; i++) {
            memcpy(store[i], prefix, (size_t)plen);
            for (k = plen; k < 20; k++)
                store[i][k] = rnd_char("abcdefghijklmnopqrstuvwxyz");
            store[i][20] = '\0';
        }
        sort_alien(words, n);
        if (shape == 1) {
            for (i = n - 2; i > 0 && strcmp(words[i], words[i + 1]) == 0; i--)
                ;
            swap_words(words, i);
        } else if (shape == 2) {
            len = (int)rnd(1, 19);
            memcpy(words[n - 1], words[n - 2], (size_t)len);
            words[n - 1][len] = '\0';
        }
    }
    put_strings(words, n);
    put_string(order);
}
