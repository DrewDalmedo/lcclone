/* Two sentences of words drawn from a small vocabulary, so that words
 * repeat within and across the sentences. Small cases use a few words of
 * a, b, aa, ab, ba and abc (prefixes of each other); full-size ones fill
 * each sentence to nearly 200 characters from a random vocabulary. */
static void sentence(int small, char vocab[][8], int v)
{
    static const char *tiny[] = {"a", "b", "aa", "ab", "ba", "abc"};
    char s[201];
    int len = 0, words = (int)rnd(1, 5), i;
    const char *w;

    for (i = 0; small ? i < words : 1; i++) {
        w = small ? tiny[rnd(0, 5)] : vocab[rnd(0, v - 1)];
        if (len + (i > 0) + (int)strlen(w) > 200)
            break;
        if (i > 0)
            s[len++] = ' ';
        memcpy(s + len, w, strlen(w));
        len += (int)strlen(w);
    }
    s[len] = '\0';
    put_string(s);
}

void gen(int small)
{
    char vocab[100][8];
    int v = small ? 0 : (int)rnd(2, 100), len, i, k;

    for (i = 0; i < v; i++) {
        len = (int)rnd(1, 7);
        for (k = 0; k < len; k++)
            vocab[i][k] = rnd_char("abcdefgh");
        vocab[i][len] = '\0';
    }
    sentence(small, vocab, v);
    sentence(small, vocab, v);
}
