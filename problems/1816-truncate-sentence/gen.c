/* A sentence and a word count k. Small cases take a few words from a
 * vocabulary of short words in mixed case, so words repeat. Full-size
 * ones fill s to 500 characters, in a shape the seed picks: random
 * words, 250 one-letter words, or a single 500-letter word. k is often
 * the whole word count, else random. */
static const char letters[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void gen(int small)
{
    static const char *tiny[] = {"a", "B", "ab", "Ab", "aB", "XY", "cab"};
    char s[501];
    int len = 0, words = 0, shape = (int)rnd(0, 2), want, wlen, i, k;
    const char *w;

    if (small) {
        want = (int)rnd(1, 6);
        while (words < want) {
            w = tiny[rnd(0, 6)];
            if (words > 0)
                s[len++] = ' ';
            memcpy(s + len, w, strlen(w));
            len += (int)strlen(w);
            words++;
        }
    } else {
        for (;;) {
            if (shape == 0)
                wlen = (int)rnd(1, 12);
            else if (shape == 1)
                wlen = 1;
            else
                wlen = 500;
            if (len + (words > 0) + wlen > 500)
                break;
            if (words > 0)
                s[len++] = ' ';
            for (i = 0; i < wlen; i++)
                s[len++] = rnd_char(letters);
            words++;
        }
    }
    s[len] = '\0';
    k = rnd_chance(30) ? words : (int)rnd(1, words);
    put_string(s);
    put_int(k);
}
