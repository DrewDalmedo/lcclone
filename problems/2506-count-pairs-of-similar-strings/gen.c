/* Words built from a few alphabets (sets of letters): each word uses
 * every letter of its alphabet at least once, plus extra copies, in a
 * random order. Small cases have up to 8 short words over subsets of
 * "abcd", so that similar words, near misses and subsets are common.
 * Full-size ones have 100 words of up to 100 letters; the seed picks a
 * few alphabets drawn from a to z, a single alphabet for every word
 * (4950 pairs), or freely random words. */
static void word_from(char *w, int len, const char *alpha)
{
    int k = (int)strlen(alpha), i, at;
    char c;

    memcpy(w, alpha, (size_t)k);
    for (i = k; i < len; i++) {
        at = (int)rnd(0, k - 1);
        w[i] = alpha[at];
    }
    for (i = len - 1; i > 0; i--) {     /* shuffle */
        at = (int)rnd(0, i);
        c = w[i];
        w[i] = w[at];
        w[at] = c;
    }
    w[len] = '\0';
}

static void pick_alphabet(char *alpha, const char *from, int most)
{
    int n = (int)strlen(from), size = (int)rnd(1, most), k = 0, i;

    for (i = 0; i < n && k < size; i++)
        if (rnd_chance(100 * (size - k) / (n - i)))
            alpha[k++] = from[i];
    alpha[k] = '\0';
}

void gen(int small)
{
    static char buf[100][101];
    static const char letters[] = "abcdefghijklmnopqrstuvwxyz";
    char alpha[8][27], *words[100];
    int n = small ? (int)rnd(1, 8) : 100, shape = (int)rnd(0, 2);
    int groups, most, len, g, i;

    if (small) {
        groups = (int)rnd(1, 4);
        for (g = 0; g < groups; g++)
            pick_alphabet(alpha[g], "abcd", 3);
    } else {
        groups = shape == 0 ? (int)rnd(2, 6) : 1;
        most = (int)rnd(1, 26);
        for (g = 0; g < groups; g++)
            pick_alphabet(alpha[g], letters, most);
    }
    for (i = 0; i < n; i++) {
        if (!small && shape == 2) {     /* any letters at all */
            pick_alphabet(alpha[0], letters, 26);
            g = 0;
        } else {
            g = (int)rnd(0, groups - 1);
        }
        len = (int)strlen(alpha[g]);
        len = (int)rnd(len, small ? len + 2 : 100);
        word_from(buf[i], len, alpha[g]);
        words[i] = buf[i];
    }
    put_strings(words, n);
}
