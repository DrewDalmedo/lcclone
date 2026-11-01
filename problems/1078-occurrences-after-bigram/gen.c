/* A text of words from a small vocabulary, so that the pair first second
 * comes up often, overlapping places included; first and second are drawn
 * from the same vocabulary, so they are often the same word. Small cases
 * use up to 10 words of a, b, ab, ba and aab (prefixes and suffixes of each
 * other), with the pair itself put in often. Full-size ones fill text to
 * nearly 1000 characters from 1 to 12 random words of 1 to 4 letters, now
 * and then one of 11 to 40 letters (never chosen as first or second); a
 * vocabulary of a single one-letter word gives 500 words and 498
 * followers. */
static int add_word(char *text, int len, const char *w, int limit)
{
    int wl = (int)strlen(w);

    if (len + (len > 0) + wl > limit)
        return -1;
    if (len > 0)
        text[len++] = ' ';
    memcpy(text + len, w, (size_t)wl);
    return len + wl;
}

void gen(int small)
{
    static const char *tiny[] = {"a", "b", "ab", "ba", "aab"};
    char vocab[12][41], text[1001];
    const char *first, *second;
    int v, words, len = 0, next, wl, i, k, jf, js;

    if (small) {
        first = tiny[rnd(0, 4)];
        second = rnd_chance(30) ? first : tiny[rnd(0, 4)];
        words = (int)rnd(1, 10);
        for (i = 0; i < words; i++) {
            if (i + 1 < words && rnd_chance(35)) { /* the pair itself */
                len = add_word(text, len, first, 1000);
                len = add_word(text, len, second, 1000);
                i++;
            } else {
                len = add_word(text, len, tiny[rnd(0, 4)], 1000);
            }
        }
        text[len] = '\0';
        put_string(text);
        put_string(first);
        put_string(second);
        return;
    }
    v = rnd_chance(15) ? 1 : (int)rnd(2, 12);
    for (i = 0; i < v; i++) {
        wl = v == 1 ? 1 : (int)rnd(1, 4);
        if (i > 0 && rnd_chance(10))
            wl = (int)rnd(11, 40);
        for (k = 0; k < wl; k++)
            vocab[i][k] = rnd_char(v <= 4 ? "ab" : "abc");
        vocab[i][wl] = '\0';
    }
    for (;;) {
        next = add_word(text, len, vocab[rnd(0, v - 1)], 1000);
        if (next < 0)
            break;
        len = next;
    }
    text[len] = '\0';
    do
        jf = (int)rnd(0, v - 1);
    while (strlen(vocab[jf]) > 10);
    do
        js = rnd_chance(20) ? jf : (int)rnd(0, v - 1);
    while (strlen(vocab[js]) > 10);
    first = vocab[jf];
    second = vocab[js];
    put_string(text);
    put_string(first);
    put_string(second);
}
