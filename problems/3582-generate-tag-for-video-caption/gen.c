/* A caption of letters (both cases) and spaces. Short captions often have
 * leading, trailing or repeated spaces, or no letters at all. The others
 * hold 95 to 105 letters (or, full size, 100 to 150 in a caption of 150
 * characters), so the 100-character cut lands around a word's edges; the
 * spaces are scattered at random among the letters. */
void gen(int small)
{
    static const char letters[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char s[151];
    int isSpace[150];
    int n, nletters, nspaces, i;

    if (small && rnd_chance(60)) {
        n = (int)rnd(1, 12);
        for (i = 0; i < n; i++)
            s[i] = rnd_chance(35) ? ' ' : rnd_char("abcAB");
        s[n] = '\0';
        put_string(s);
        return;
    }
    if (small) {
        nletters = (int)rnd(95, 105);
        nspaces = (int)rnd(0, 150 - nletters < 45 ? 150 - nletters : 45);
    } else {
        nletters = rnd_chance(20) ? 150 : (int)rnd(100, 150);
        nspaces = 150 - nletters;
    }
    n = nletters + nspaces;
    for (i = 0; i < n; i++)
        isSpace[i] = i < nspaces;
    rnd_shuffle(isSpace, n);
    for (i = 0; i < n; i++)
        s[i] = isSpace[i] ? ' ' : rnd_char(letters);
    s[n] = '\0';
    put_string(s);
}
