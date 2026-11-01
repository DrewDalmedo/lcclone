/* Words that often bookend each other. The mixed shape draws each word as
 * a copy of an earlier word, a short root repeated (so shorter repeats
 * bookend longer ones, often overlapping), an earlier word written twice
 * with up to two letters between, or a random word. Small cases: up to 8
 * words of up to 5 letters from "ab" or "abc", all in the mixed shape.
 * Full size: mostly 50 words of up to 10 letters, in a shape the seed
 * picks: the mix above, all 50 words equal (the largest answer), words of
 * the letter a only, near misses (a's with a b at a random place), or
 * random words over the whole alphabet. */
static void random_word(char *w, int len, const char *letters)
{
    int i;

    for (i = 0; i < len; i++)
        w[i] = rnd_char(letters);
    w[len] = '\0';
}

static void root_repeat(char *w, int len, const char *letters)
{
    char root[3];
    int r = (int)rnd(1, 3), i;

    for (i = 0; i < r; i++)
        root[i] = rnd_char(letters);
    for (i = 0; i < len; i++)
        w[i] = root[i % r];
    w[len] = '\0';
}

static void mixed_word(char words[][11], int i, int maxlen,
                       const char *letters)
{
    char *w = words[i];
    int kind = (int)rnd(0, 9), len, at, mid, k;

    if (kind < 2 && i > 0) {
        at = (int)rnd(0, i - 1);
        strcpy(w, words[at]);
        return;
    }
    if (kind >= 5 && kind < 7 && i > 0) {
        at = (int)rnd(0, i - 1);
        len = (int)strlen(words[at]);
        mid = (int)rnd(0, 2);
        if (2 * len + mid <= maxlen) {
            memcpy(w, words[at], (size_t)len);
            for (k = 0; k < mid; k++)
                w[len + k] = rnd_char(letters);
            memcpy(w + len + mid, words[at], (size_t)len + 1);
            return;
        }
    }
    len = (int)rnd(1, maxlen);
    if (kind >= 2 && kind < 5)
        root_repeat(w, len, letters);
    else
        random_word(w, len, letters);
}

void gen(int small)
{
    char words[50][11];
    char *ptrs[50];
    const char *letters = "ab";
    int maxlen = small ? 5 : 10;
    int n, shape, i, len, at;

    if (small)
        n = (int)rnd(1, 8);
    else
        n = rnd_chance(80) ? 50 : (int)rnd(1, 50);
    if (small && rnd_chance(50))
        letters = "abc";
    shape = small ? 0 : (int)rnd(0, 4);
    for (i = 0; i < n; i++) {
        if (shape == 0) {
            mixed_word(words, i, maxlen, letters);
        } else if (shape == 1) {
            if (i == 0) {
                len = (int)rnd(1, 10);
                random_word(words[0], len, "abcdefghijklmnopqrstuvwxyz");
            } else {
                strcpy(words[i], words[0]);
            }
        } else if (shape == 2) {
            len = (int)rnd(1, 10);
            memset(words[i], 'a', (size_t)len);
            words[i][len] = '\0';
        } else if (shape == 3) {
            len = (int)rnd(5, 10);
            memset(words[i], 'a', (size_t)len);
            words[i][len] = '\0';
            if (rnd_chance(60)) {
                at = (int)rnd(0, len - 1);
                words[i][at] = 'b';
            }
        } else {
            len = (int)rnd(1, 10);
            random_word(words[i], len, "abcdefghijklmnopqrstuvwxyz");
        }
        ptrs[i] = words[i];
    }
    put_strings(ptrs, n);
}
