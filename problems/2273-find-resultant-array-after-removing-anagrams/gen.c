/* Lists of words in which neighbours are often anagrams. Small cases have
 * 1 to 8 words of 1 to 3 letters a and b (so "aab" and "abb" meet often),
 * each word half the time a reshuffle of the one before it. Full-size
 * cases have 100 words of up to 10 letters, in one of four shapes picked
 * by the seed: one long run of reshuffles of a single 10-letter word; two
 * anagram classes taking turns, so that nothing is struck; runs of
 * reshuffles of random words over a, b and c; or random words over the
 * whole alphabet. */

/* Puts the letters of s in a random order. */
static void reshuffle(char *s)
{
    int n = (int)strlen(s), i, j;
    char c;

    for (i = n - 1; i > 0; i--) {
        j = (int)rnd(0, i);
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

static void random_word(char *s, int maxlen, const char *letters)
{
    int len = (int)rnd(1, maxlen), i;

    for (i = 0; i < len; i++)
        s[i] = rnd_char(letters);
    s[len] = '\0';
}

void gen(int small)
{
    static char store[100][11];
    char *words[100], base[2][11];
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = small ? 4 : (int)rnd(0, 3), i;

    if (shape == 0) {
        /* Ten letters, the longest word allowed. */
        for (i = 0; i < 10; i++)
            base[0][i] = rnd_char("abcdefghijklmnopqrstuvwxyz");
        base[0][10] = '\0';
    }
    if (shape == 1) {
        /* Two different letter counts: the second word is the first
         * with one letter changed to z, which the first doesn't have. */
        random_word(base[0], 10, "abcde");
        strcpy(base[1], base[0]);
        base[1][0] = 'z';
    }
    for (i = 0; i < n; i++) {
        words[i] = store[i];
        if (shape == 0 || shape == 1) {
            strcpy(store[i], base[shape == 1 ? i % 2 : 0]);
            reshuffle(store[i]);
        } else if (i > 0 && rnd_chance(shape == 3 ? 5 : 50)) {
            strcpy(store[i], store[i - 1]);
            reshuffle(store[i]);
        } else if (shape == 2) {
            random_word(store[i], 10, "abc");
        } else if (shape == 3) {
            random_word(store[i], 10, "abcdefghijklmnopqrstuvwxyz");
        } else {
            random_word(store[i], 3, "ab");
        }
    }
    put_strings(words, n);
}
