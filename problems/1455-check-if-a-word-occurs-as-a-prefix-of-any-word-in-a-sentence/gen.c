/* Small cases: a few words that are prefixes of each other (a, ab, abc, b,
 * ba, ...) and a short searchWord, so that words shorter than searchWord,
 * equal to it and longer than it all come up. Full-size cases fill the
 * sentence to nearly 100 characters, in a shape picked by the seed: random
 * words over two letters; words holding searchWord after their first
 * letter; up to 50 one-letter words; or the beginnings of a 10-letter
 * searchWord. In the last three, only the last word can match, and does
 * half the time. */
static char sent[101];
static int slen;

/* Appends w as the next word if the sentence stays within limit. */
static int add(const char *w, int limit)
{
    int k = (int)strlen(w);

    if (slen + (slen > 0) + k > limit)
        return 0;
    if (slen > 0)
        sent[slen++] = ' ';
    memcpy(sent + slen, w, (size_t)k);
    slen += k;
    sent[slen] = '\0';
    return 1;
}

static void rnd_word(char *w, int lo, int hi, const char *letters)
{
    int k = (int)rnd(lo, hi), i;

    for (i = 0; i < k; i++)
        w[i] = rnd_char(letters);
    w[k] = '\0';
}

/* A letter of letters other than c. */
static char not_c(const char *letters, char c)
{
    char x;

    do
        x = rnd_char(letters);
    while (x == c);
    return x;
}

void gen(int small)
{
    static const char *tiny[] = {"a", "ab", "abc", "b", "ba", "bab", "aab"};
    static const char *abc = "abc";
    char sw[11], w[101], last[101];
    int shape, words, i, k, n;

    slen = 0;
    sent[0] = '\0';
    if (small) {
        words = (int)rnd(1, 6);
        for (i = 0; i < words; i++)
            add(tiny[rnd(0, 6)], 100);
        if (rnd_chance(40))
            strcpy(sw, tiny[rnd(0, 6)]);
        else
            rnd_word(sw, 1, 3, "ab");
        put_string(sent);
        put_string(sw);
        return;
    }
    shape = (int)rnd(0, 3);
    if (shape == 0) {
        rnd_word(sw, 1, 4, "ab");
        do
            rnd_word(w, 1, 8, "ab");
        while (add(w, 100));
        put_string(sent);
        put_string(sw);
        return;
    }
    /* searchWord, and a last word that begins with it or doesn't. */
    if (shape == 1)
        rnd_word(sw, 3, 10, abc);
    else if (shape == 2)
        rnd_word(sw, 1, 2, abc);
    else
        rnd_word(sw, 10, 10, abc);
    n = (int)strlen(sw);
    strcpy(last, sw);
    if (rnd_chance(50)) {
        k = (int)rnd(0, shape == 2 ? 0 : 3);
        for (i = 0; i < k; i++)
            last[n + i] = rnd_char(abc);
        last[n + k] = '\0';
    } else {
        last[n - 1] = not_c(abc, sw[n - 1]);
    }
    for (;;) {
        if (shape == 1) {
            /* First letter differs from searchWord's; searchWord follows. */
            w[0] = not_c(abc, sw[0]);
            k = (int)rnd(0, 2);
            for (i = 1; i <= k; i++)
                w[i] = rnd_char(abc);
            memcpy(w + k + 1, sw, (size_t)n);
            w[k + 1 + n] = '\0';
            if (rnd_chance(30))
                w[k + 1 + n - (int)rnd(1, n - 1)] = '\0';
        } else if (shape == 2) {
            w[0] = not_c(abc, sw[0]);
            w[1] = '\0';
        } else {
            /* A beginning of searchWord, shorter than it. */
            k = (int)rnd(1, n - 1);
            memcpy(w, sw, (size_t)k);
            w[k] = '\0';
        }
        if (!add(w, 100 - 1 - (int)strlen(last)))
            break;
    }
    add(last, 100);
    put_string(sent);
    put_string(sw);
}
