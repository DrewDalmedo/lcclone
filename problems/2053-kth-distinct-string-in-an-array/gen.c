/* Small cases have 1 to 8 strings, either from a, b, aa, ab, ba and abc
 * (prefixes and anagrams of each other) or of 1 or 2 letters of "abc", so
 * that repeats are common. Full-size cases have 1000 strings, in a shape
 * the seed picks: 5-letter strings, nearly all different; a vocabulary of
 * 2 to 600 random strings; one string 257, 513 or so many times among
 * nearly-different others; or strings like q, aq, aaq (a letter after a
 * run of a's) mixed with random 5-letter ones. k is the number of
 * one-offs, one more than that, or a random rank up to it. */
static const char letters[] = "abcdefghijklmnopqrstuvwxyz";

static void random_word(char *w, int len, const char *pool)
{
    int i;

    for (i = 0; i < len; i++)
        w[i] = rnd_char(pool);
    w[len] = '\0';
}

void gen(int small)
{
    static const char *tiny[] = {"a", "b", "aa", "ab", "ba", "abc"};
    static const int heavy_counts[] = {257, 513, 769};
    static char words[1000][6], vocab[600][6];
    char *ptrs[1000];
    int n = small ? (int)rnd(1, 8) : 1000, d = 0, i, j, k, v, len, shape;
    int heavy, alone, mode, top;

    if (small) {
        shape = (int)rnd(0, 1);
        for (i = 0; i < n; i++) {
            if (shape == 0) {
                strcpy(words[i], tiny[rnd(0, 5)]);
            } else {
                len = (int)rnd(1, 2);
                random_word(words[i], len, "abc");
            }
        }
    } else {
        shape = (int)(gen_seed() % 4);
        if (shape == 0) {
            for (i = 0; i < n; i++)
                random_word(words[i], 5, letters);
        } else if (shape == 1) {
            v = (int)rnd(2, 600);
            for (i = 0; i < v; i++) {
                len = (int)rnd(1, 5);
                random_word(vocab[i], len, letters);
            }
            for (i = 0; i < n; i++)
                strcpy(words[i], vocab[rnd(0, v - 1)]);
        } else if (shape == 2) {
            heavy = rnd_chance(60) ? heavy_counts[rnd(0, 2)]
                                   : (int)rnd(257, 900);
            len = (int)rnd(1, 5);
            random_word(vocab[0], len, letters);
            for (i = 0; i < n; i++) {
                if (i < heavy) {
                    strcpy(words[i], vocab[0]);
                } else {
                    len = (int)rnd(3, 5);
                    random_word(words[i], len, letters);
                }
            }
            /* Spread the copies: shuffle the entries. */
            for (i = n - 1; i > 0; i--) {
                j = (int)rnd(0, i);
                memcpy(vocab[1], words[i], 6);
                memcpy(words[i], words[j], 6);
                memcpy(words[j], vocab[1], 6);
            }
        } else {
            for (i = 0; i < n; i++) {
                if (rnd_chance(30)) {
                    random_word(words[i], 5, letters);
                } else {
                    len = (int)rnd(0, 4);
                    memset(words[i], 'a', (size_t)len);
                    words[i][len] = rnd_char(letters);
                    words[i][len + 1] = '\0';
                }
            }
        }
    }

    for (i = 0; i < n; i++) {
        alone = 1;
        for (j = 0; j < n && alone; j++)
            if (j != i && strcmp(words[i], words[j]) == 0)
                alone = 0;
        d += alone;
        ptrs[i] = words[i];
    }
    mode = (int)rnd(0, 2);
    top = d > 0 ? d : 1;
    if (mode == 0)
        k = top;
    else if (mode == 1)
        k = d + 1 <= n ? d + 1 : n;
    else
        k = (int)rnd(1, top);
    put_strings(ptrs, n);
    put_int(k);
}
