/* Strings built from runs of one repeated letter. Small cases join runs of
 * 1 to 5 copies of a, b or c, so that runs of exactly 2 and 3 come up
 * often, back to back. Full-size cases have 10^5 letters in one of four
 * shapes, picked by the seed: one letter throughout; half the length with
 * no three in a row, then a single long run (slow for "find three in a
 * row, strike one, start over"); random runs, some of them hundreds of
 * letters long; or no three in a row anywhere, so that nothing is struck. */

/* A letter of `letters` other than `avoid`. */
static char pick(const char *letters, char avoid)
{
    char c;

    do
        c = rnd_char(letters);
    while (c == avoid);
    return c;
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 14) : 100000;
    int shape = small ? 4 : (int)rnd(0, 3);
    const char *letters = "abcdefghijklmnopqrstuvwxyz";
    char *s = malloc((size_t)n + 1), c = 0;
    int len = 0, run;

    if (small)
        letters = rnd_chance(50) ? "ab" : "abc";
    while (len < n) {
        if (shape == 0) {
            c = rnd_char(letters);
            run = n;
        } else if (shape == 1) {
            c = pick(letters, c);
            run = len < n / 2 ? (int)rnd(1, 2) : n - len;
        } else if (shape == 2) {
            c = rnd_char(letters);
            run = rnd_chance(3) ? (int)rnd(200, 1000) : (int)rnd(1, 4);
        } else if (shape == 3) {
            c = pick(letters, c);
            run = (int)rnd(1, 2);
        } else {
            c = rnd_char(letters);
            run = (int)rnd(1, 5);
        }
        if (run > n - len)
            run = n - len;
        memset(s + len, c, (size_t)run);
        len += run;
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
