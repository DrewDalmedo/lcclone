/* Words built around pref, each in one of four ways: pref plus a few more
 * letters (or pref itself), a shorter start of pref, pref with one letter
 * changed (often its last) plus a few more, or random letters; an entry
 * sometimes repeats an earlier one. Small cases: up to 8 words of at most
 * 5 letters, and pref of 1 to 3 letters, all from "ab". Full-size cases
 * have 100 words; the seed picks the slowest shape (pref of 100 letters
 * and words of 100 letters that agree with it on the first 99, matching
 * or not at the last), a long pref, or a short one. */
static void random_letters(char *w, int len, const char *letters)
{
    int i;

    for (i = 0; i < len; i++)
        w[i] = rnd_char(letters);
    w[len] = '\0';
}

static void make_word(char *w, const char *pref, int plen, int max,
                      const char *letters)
{
    int shape = (int)rnd(0, 3), len, pos, extra;
    char c;

    if (shape == 0 && plen <= max) {
        extra = (int)rnd(0, max - plen < 2 ? max - plen : 2);
        memcpy(w, pref, (size_t)plen);
        random_letters(w + plen, extra, letters);
    } else if (shape == 1 && plen >= 2) {
        len = (int)rnd(1, plen - 1 < max ? plen - 1 : max);
        memcpy(w, pref, (size_t)len);
        w[len] = '\0';
    } else if (shape == 2 && plen <= max) {
        pos = rnd_chance(50) ? plen - 1 : (int)rnd(0, plen - 1);
        do
            c = rnd_char(letters);
        while (c == pref[pos]);
        extra = (int)rnd(0, max - plen < 2 ? max - plen : 2);
        memcpy(w, pref, (size_t)plen);
        w[pos] = c;
        random_letters(w + plen, extra, letters);
    } else {
        len = (int)rnd(1, max);
        random_letters(w, len, letters);
    }
}

void gen(int small)
{
    static char buf[100][101];
    char pref[101];
    char *words[100];
    int n, plen, shape, i, j;

    if (small) {
        n = (int)rnd(1, 8);
        plen = (int)rnd(1, 3);
        random_letters(pref, plen, "ab");
        for (i = 0; i < n; i++) {
            if (i > 0 && rnd_chance(15)) {
                j = (int)rnd(0, i - 1);
                strcpy(buf[i], buf[j]);
            } else {
                make_word(buf[i], pref, plen, 5, "ab");
            }
            words[i] = buf[i];
        }
        put_strings(words, n);
        put_string(pref);
        return;
    }
    n = 100;
    shape = (int)(gen_seed() % 3);
    if (shape == 0) {
        random_letters(pref, 100, "abcdefghijklmnopqrstuvwxyz");
        for (i = 0; i < n; i++) {
            memcpy(buf[i], pref, 99);
            if (rnd_chance(50))
                buf[i][99] = pref[99];
            else
                buf[i][99] = pref[99] == 'z' ? 'a' : (char)(pref[99] + 1);
            buf[i][100] = '\0';
            words[i] = buf[i];
        }
    } else {
        plen = shape == 1 ? (int)rnd(1, 100) : (int)rnd(1, 10);
        random_letters(pref, plen, "abc");
        for (i = 0; i < n; i++) {
            make_word(buf[i], pref, plen, 100, "abc");
            words[i] = buf[i];
        }
    }
    put_strings(words, n);
    put_string(pref);
}
