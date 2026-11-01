/* s and words over a few letters, so that words often almost match: each
 * word is a prefix of s, a prefix of s with its last letter changed, s
 * with letters added, a random string, or a copy of an earlier word.
 * Small cases have up to 8 words and s of 1 to 4 letters; full-size ones
 * have 1000 words, with the seed picking the shape: every word a prefix of
 * s, a random mix, or near misses only (answer 0). */
static char w[1000][11];

/* Writes into out a word of the given kind (0 to 4) built around s. */
static void make_word(char *out, const char *s, int n, const char *abc,
                      int kind, int i)
{
    int len, k, j;
    char c;

    if (kind == 4 && i == 0)
        kind = 0;
    if (kind == 0 || kind == 1) {
        len = (int)rnd(1, n);
        memcpy(out, s, (size_t)len);
        if (kind == 1) {
            do
                c = rnd_char(abc);
            while (c == s[len - 1]);
            out[len - 1] = c;
        }
    } else if (kind == 2) {
        if (n == 10) {
            /* s can't be extended; change a letter of a copy instead */
            make_word(out, s, n, abc, 1, i);
            return;
        }
        len = (int)rnd(n + 1, n + 2 < 10 ? n + 2 : 10);
        memcpy(out, s, (size_t)n);
        for (k = n; k < len; k++)
            out[k] = rnd_char(abc);
    } else if (kind == 3) {
        len = (int)rnd(1, 5);
        for (k = 0; k < len; k++)
            out[k] = rnd_char(abc);
    } else {
        j = (int)rnd(0, i - 1);
        len = (int)strlen(w[j]);
        memcpy(out, w[j], (size_t)len);
    }
    out[len] = '\0';
}

void gen(int small)
{
    char s[11], *ptr[1000];
    const char *abc = rnd_chance(70) ? "ab" : "abc";
    int count, n, shape, kind, i, k;

    if (small) {
        count = (int)rnd(1, 8);
        n = (int)rnd(1, 4);
        shape = 1;
    } else {
        count = 1000;
        n = rnd_chance(60) ? 10 : (int)rnd(1, 9);
        shape = (int)rnd(0, 2);
    }
    for (k = 0; k < n; k++)
        s[k] = rnd_char(abc);
    s[n] = '\0';
    for (i = 0; i < count; i++) {
        if (shape == 0)
            kind = 0;
        else if (shape == 1)
            kind = (int)rnd(0, 4);
        else
            kind = (int)rnd(1, 2);
        make_word(w[i], s, n, abc, kind, i);
        ptr[i] = w[i];
    }
    put_strings(ptr, count);
    put_string(s);
}
