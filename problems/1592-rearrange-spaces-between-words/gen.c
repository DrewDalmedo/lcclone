/* A line of w words with spaces before, between and after them. Small
 * cases have up to 4 words of 1 to 3 letters from a, b and c, with 1 to 3
 * spaces in each gap and up to 2 at each end. Full-size ones are mostly
 * 100 characters long, with one word, two, fifty or some number between:
 * the letters and the spaces beyond the one each gap needs are dealt out
 * at random, and some seeds put all the extra spaces in one place (before
 * the first word, in one gap, or after the last word). */
static void lay_out(char *s, const int *wordlen, const int *slot, int w,
                    const char *letters)
{
    int len = 0, i, k;

    for (k = 0; k < slot[0]; k++)
        s[len++] = ' ';
    for (i = 0; i < w; i++) {
        for (k = 0; k < wordlen[i]; k++)
            s[len++] = rnd_char(letters);
        for (k = 0; k < slot[i + 1]; k++)
            s[len++] = ' ';
    }
    s[len] = '\0';
}

void gen(int small)
{
    int wordlen[50], slot[51], w, n, s, i, at, shape, one_place;
    char text[101];

    if (small) {
        w = (int)rnd(1, 4);
        for (i = 0; i < w; i++)
            wordlen[i] = (int)rnd(1, 3);
        for (i = 0; i <= w; i++)
            slot[i] = (int)(i == 0 || i == w ? rnd(0, 2) : rnd(1, 3));
        lay_out(text, wordlen, slot, w, "abc");
        put_string(text);
        return;
    }
    shape = (int)rnd(0, 4);
    w = shape == 0 ? 1 : shape == 1 ? 2 : shape == 2 ? 50 : (int)rnd(3, 49);
    n = 100;
    if (w < 50 && rnd_chance(25))
        n = (int)rnd(2 * w - 1, 100);
    s = (int)rnd(w - 1, n - w);
    for (i = 0; i < w; i++)
        wordlen[i] = 1;
    for (i = w; i < n - s; i++) {
        at = (int)rnd(0, w - 1);
        wordlen[at]++;
    }
    for (i = 0; i <= w; i++)
        slot[i] = i == 0 || i == w ? 0 : 1;
    one_place = rnd_chance(30) ? (int)rnd(0, w) : -1;
    for (i = w - 1; i < s; i++) {
        at = one_place >= 0 ? one_place : (int)rnd(0, w);
        slot[at]++;
    }
    lay_out(text, wordlen, slot, w, "abcdefghijklmnopqrstuvwxyz");
    put_string(text);
}
