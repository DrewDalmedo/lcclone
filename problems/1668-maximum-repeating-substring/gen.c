/* sequence is glued together from pieces of word: runs of whole copies,
 * the first few letters of word, its last few letters, loose letters,
 * and, when word ends the way it begins, a copy of word whose last letters
 * begin a chain of copies (the chain a scan that jumps past each match
 * misses). Small cases use words of 1 to 4 letters, often ones that end
 * the way they begin, and a sequence of up to 14 letters. Full-size ones have 100
 * letters, in a shape the seed picks: pieces of a short word, often one
 * from a list of words that end the way they begin; one letter throughout
 * with a word of that letter (the answer is 100 / word.length) or of that
 * letter but the last (no match, but every comparison runs almost to the
 * end); loose letters with the longest chain at the very end; or a long
 * word, which fits at most a few times. */
static const char *bordered[] = {"aba", "abcab", "aabaa", "abaab", "abaaba",
                                 "aab", "abcabca", "aa", "abab"};
static const char *short_bordered[] = {"aba", "bab", "abaa", "abca", "aaba",
                                       "abba"};

/* The smallest d with 0 < d < m such that word is d letters, repeated
 * over and over and cut off after m letters; 0 if there is none. Then
 * word's first d letters followed by a copy of word begin with word too. */
static int period(const char *w, int m)
{
    int d, i;

    for (d = 1; d < m; d++) {
        for (i = 0; i + d < m && w[i] == w[i + d]; i++)
            ;
        if (i + d == m)
            return d;
    }
    return 0;
}

static void add_run(char *s, int *len, int n, const char *w, int m, int k)
{
    int i;

    for (i = 0; i < k * m && *len < n; i++)
        s[(*len)++] = w[i % m];
}

static void add_pieces(char *s, int *len, int n, const char *w, int m,
                       const char *alpha, int max_run)
{
    int per = period(w, m), kind, k, cut, i;

    while (*len < n) {
        kind = (int)rnd(0, 9);
        k = (int)rnd(1, max_run);
        if (kind < 5) {
            add_run(s, len, n, w, m, k);
        } else if (kind < 6 || (kind < 7 && per == 0)) {
            cut = (int)rnd(1, m);
            for (i = 0; i < cut && *len < n; i++)
                s[(*len)++] = w[i];
        } else if (kind < 7) {
            /* A copy of word whose last letters begin a chain of k. */
            for (i = 0; i < per && *len < n; i++)
                s[(*len)++] = w[i];
            add_run(s, len, n, w, m, k);
        } else if (kind < 8) {
            cut = (int)rnd(0, m - 1);
            for (i = cut; i < m && *len < n; i++)
                s[(*len)++] = w[i];
        } else {
            s[(*len)++] = rnd_char(alpha);
        }
    }
}

void gen(int small)
{
    char s[101], w[101];
    const char *alpha;
    int n, m, len = 0, shape, k, i;
    char c;

    if (small) {
        alpha = rnd_chance(70) ? "ab" : "abc";
        if (rnd_chance(40)) {
            strcpy(w, short_bordered[rnd(0, 5)]);
            m = (int)strlen(w);
        } else {
            m = (int)rnd(1, 4);
            for (i = 0; i < m; i++)
                w[i] = rnd_char(alpha);
        }
        n = (int)rnd(1, 14);
        add_pieces(s, &len, n, w, m, alpha, 4);
    } else {
        n = 100;
        shape = (int)(gen_seed() % 4);
        if (shape == 0) {
            alpha = "abc";
            if (rnd_chance(60)) {
                strcpy(w, bordered[rnd(0, 8)]);
                m = (int)strlen(w);
            } else {
                m = (int)rnd(1, 6);
                for (i = 0; i < m; i++)
                    w[i] = rnd_char("ab");
            }
            add_pieces(s, &len, n, w, m, alpha, 12);
        } else if (shape == 1) {
            c = rnd_char("abcdefghijklmnopqrstuvwxyz");
            m = (int)rnd(1, 100);
            for (i = 0; i < m; i++)
                w[i] = c;
            if (rnd_chance(50))
                w[m - 1] = c == 'z' ? 'a' : (char)(c + 1);
            memset(s, c, (size_t)n);
            len = n;
        } else if (shape == 2) {
            m = (int)rnd(1, 8);
            for (i = 0; i < m; i++)
                w[i] = rnd_char("abc");
            k = (int)rnd(n / m / 2, n / m);
            while (len < n - k * m)
                s[len++] = rnd_char("abcd");
            for (i = 0; i < k * m; i++)
                s[len++] = w[i % m];
        } else {
            m = (int)rnd(30, 100);
            alpha = "ab";
            for (i = 0; i < m; i++)
                w[i] = rnd_char(alpha);
            add_pieces(s, &len, n, w, m, alpha, 3);
        }
    }
    s[n] = '\0';
    w[m] = '\0';
    put_string(s);
    put_string(w);
}
