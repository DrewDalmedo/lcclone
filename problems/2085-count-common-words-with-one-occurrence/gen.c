/* Small cases: 1 to 8 words per list from the first 2 to 8 of a, b, aa,
 * ab, ba, abc, c and abcd (some prefixes of others). Each list goes
 * through the words in a random order, with a random word instead now and
 * then, so words repeat within and across the lists.
 * Full-size cases have 1000 words per list; the seed picks words drawn
 * from a random vocabulary, or 1000 different words that both lists hold
 * once each in different orders (the answer is 1000). Full-size words
 * share a random prefix and end in three letters that set them apart, so
 * some are 30 letters long and differ only near the end. */
static void make_word(char *w, const char *prefix, int code)
{
    int len = (int)strlen(prefix);

    memcpy(w, prefix, (size_t)len);
    w[len] = (char)('a' + code / 676);
    w[len + 1] = (char)('a' + code / 26 % 26);
    w[len + 2] = (char)('a' + code % 26);
    w[len + 3] = '\0';
}

static void small_list(int vocab)
{
    static const char *tiny[] = {"a", "b", "aa", "ab", "ba", "abc", "c",
                                 "abcd"};
    char *list[8];
    int perm[8], n = (int)rnd(1, 8), i, pick;

    for (i = 0; i < vocab; i++)
        perm[i] = i;
    rnd_shuffle(perm, vocab);
    for (i = 0; i < n; i++) {
        pick = rnd_chance(30) ? (int)rnd(0, vocab - 1) : perm[i % vocab];
        list[i] = (char *)tiny[pick];
    }
    put_strings(list, n);
}

void gen(int small)
{
    static char w1[1000][31], w2[1000][31];
    char *p1[1000], *p2[1000], prefix[28];
    int order[1000], n = 1000, plen, vocab, i, code;

    if (small) {
        vocab = (int)rnd(2, 8);
        small_list(vocab);
        small_list(vocab);
        return;
    }
    plen = (int)rnd(0, 27);
    for (i = 0; i < plen; i++)
        prefix[i] = rnd_char("abcdefghijklmnopqrstuvwxyz");
    prefix[plen] = '\0';
    if (rnd_chance(50)) {
        for (i = 0; i < n; i++)
            order[i] = i;
        rnd_shuffle(order, n);
        for (i = 0; i < n; i++) {
            make_word(w1[i], prefix, order[i]);
            make_word(w2[i], prefix, order[n - 1 - i]);
        }
    } else {
        vocab = (int)rnd(500, 2000);
        for (i = 0; i < n; i++) {
            code = (int)rnd(0, vocab - 1);
            make_word(w1[i], prefix, code);
            code = (int)rnd(0, vocab - 1);
            make_word(w2[i], prefix, code);
        }
    }
    for (i = 0; i < n; i++) {
        p1[i] = w1[i];
        p2[i] = w2[i];
    }
    put_strings(p1, n);
    put_strings(p2, n);
}
