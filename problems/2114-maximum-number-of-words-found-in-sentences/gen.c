/* Small cases are 1 to 5 sentences of 1 to 6 words of up to 5 letters
 * from "ab", so that ties, one-word sentences and short sentences with
 * many words come up often. Full-size ones are 100 sentences, in a shape
 * the seed picks: random lengths and word sizes; long words everywhere
 * but 50 one-letter words in the last sentence; a single 100-letter word
 * in each; or the same number of words in every sentence. */
static char text[100][101];

/* Writes into s up to maxwords words of minw to maxw letters, separated by
 * single spaces, stopping when the next word wouldn't fit in len
 * characters; the first word is cut to len letters if need be. */
static void sentence(char *s, int len, int maxwords, int minw, int maxw,
                     const char *letters)
{
    int n = 0, words = 0, w, k;

    while (words < maxwords) {
        w = (int)rnd(minw, maxw);
        if (words > 0) {
            if (n + 1 + w > len)
                break;
            s[n++] = ' ';
        } else if (w > len) {
            w = len;
        }
        for (k = 0; k < w; k++)
            s[n++] = rnd_char(letters);
        words++;
    }
    s[n] = '\0';
}

void gen(int small)
{
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    char *sentences[100];
    int m, i, len, maxw, k;

    if (small) {
        m = (int)rnd(1, 5);
        for (i = 0; i < m; i++) {
            k = (int)rnd(1, 6);
            maxw = (int)rnd(1, 5);
            sentence(text[i], 100, k, 1, maxw, "ab");
        }
    } else {
        m = 100;
        switch (gen_seed() % 4) {
        case 0: /* random lengths and word sizes */
            for (i = 0; i < m; i++) {
                len = (int)rnd(1, 100);
                maxw = (int)rnd(1, 12);
                sentence(text[i], len, 50, 1, maxw, alphabet);
            }
            break;
        case 1: /* the last sentence has 50 words, the others 33 at most */
            for (i = 0; i < m - 1; i++)
                sentence(text[i], 100, 50, 2, 10, alphabet);
            sentence(text[m - 1], 100, 50, 1, 1, alphabet);
            break;
        case 2: /* one word of 100 letters in each */
            for (i = 0; i < m; i++)
                sentence(text[i], 100, 1, 100, 100, alphabet);
            break;
        default: /* k one-letter words in every sentence */
            k = (int)rnd(1, 50);
            for (i = 0; i < m; i++)
                sentence(text[i], 100, k, 1, 1, alphabet);
            break;
        }
    }
    for (i = 0; i < m; i++)
        sentences[i] = text[i];
    put_strings(sentences, m);
}
