/* A scrambled sentence. Small cases have 1 to 9 words (mostly up to 5) of
 * 1 to 3 letters from a, b, A and B, so that repeated words and words that
 * differ only in case come up often. Full-size cases have 1 to 9 words of
 * letters of both cases filling s to 200 characters (or nearly, when every
 * word is the same). The seed picks the order of the words: shuffled,
 * reversed or as in the original sentence; or, in full-size cases, every
 * word the same letters, shuffled. */
void gen(int small)
{
    static const char letters[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char words[9][200], s[201];
    int len[9], order[9], k, shape, total, pos = 0, i, j, w;

    if (small)
        k = rnd_chance(75) ? (int)rnd(1, 5) : (int)rnd(6, 9);
    else
        k = (int)rnd(1, 9);
    shape = (int)rnd(0, 3);
    if (small) {
        for (i = 0; i < k; i++) {
            len[i] = (int)rnd(1, 3);
            for (j = 0; j < len[i]; j++)
                words[i][j] = rnd_char("abAB");
        }
    } else {
        /* k digits and k - 1 spaces leave 201 - 2k letters for 200 in all. */
        total = 201 - 2 * k;
        if (shape == 3) {
            len[0] = total / k;
            for (j = 0; j < len[0]; j++)
                words[0][j] = rnd_char(letters);
            for (i = 1; i < k; i++) {
                len[i] = len[0];
                memcpy(words[i], words[0], (size_t)len[0]);
            }
        } else {
            for (i = 0; i < k; i++)
                len[i] = 1;
            for (j = k; j < total; j++)
                len[rnd(0, k - 1)]++;
            for (i = 0; i < k; i++)
                for (j = 0; j < len[i]; j++)
                    words[i][j] = rnd_char(letters);
        }
    }
    for (i = 0; i < k; i++)
        order[i] = shape == 1 ? k - 1 - i : i;
    if (shape == 0 || shape == 3)
        rnd_shuffle(order, k);
    for (i = 0; i < k; i++) {
        w = order[i];
        if (i > 0)
            s[pos++] = ' ';
        memcpy(s + pos, words[w], (size_t)len[w]);
        pos += len[w];
        s[pos++] = (char)('1' + w);
    }
    s[pos] = '\0';
    put_string(s);
}
