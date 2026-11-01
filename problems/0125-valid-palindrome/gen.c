/* Most cases hide a palindrome of letters and digits among other
 * characters: the letters get random cases, and sometimes one character
 * is changed so that it no longer matches. The rest are random. Small
 * cases use a few characters, among them 0 and P, and 9 and Y, whose codes
 * differ by 32, and the neighbours of the letter and digit ranges. Full-size
 * ones are 2 * 10^5 characters: a palindrome, a palindrome broken in the
 * middle, nothing but punctuation and spaces, or random printable ASCII. */
static char upper(char c)
{
    return c >= 'a' && c <= 'z' ? (char)(c - 'a' + 'A') : c;
}

/* Fills s[0..n) with k characters of a palindrome over `alnum` (lowercase
 * letters and digits), letters in random cases, at random positions, and
 * characters of `seps` everywhere else. With `broken` 1, one character
 * of the palindrome that has a partner is changed to something else; with
 * 2, the character just before the middle is (so a check from both ends
 * finds the mismatch last). */
static void hide(char *s, int n, int k, const char *alnum, const char *seps,
                 int broken)
{
    char *p = malloc((size_t)k + 1);
    int i, j, left = k;
    char c;

    for (i = 0; i < (k + 1) / 2; i++)
        p[i] = p[k - 1 - i] = rnd_char(alnum);
    if (broken && k >= 2) {
        j = broken == 2 ? k / 2 - 1 : (int)rnd(0, k / 2 - 1);
        if (broken == 1 && rnd_chance(50))
            j = k - 1 - j;
        do
            c = rnd_char(alnum);
        while (c == p[k - 1 - j]);
        p[j] = c;
    }
    for (i = 0; i < k; i++)
        if (rnd_chance(50))
            p[i] = upper(p[i]);
    for (i = 0, j = 0; i < n; i++) {
        if (rnd(1, n - i) <= left) {
            s[i] = p[j++];
            left--;
        } else {
            s[i] = rnd_char(seps);
        }
    }
    free(p);
}

void gen(int small)
{
    static const char seps[] =
        " !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    static const char alnum[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    int n = small ? (int)rnd(1, 10) : 200000, i, shape;
    char *s = malloc((size_t)n + 1);

    if (small) {
        shape = (int)rnd(0, 9);
        if (shape < 7)
            hide(s, n, (int)rnd(0, n), "ab0p9y", " ,_/:@[`{", shape >= 4);
        else
            for (i = 0; i < n; i++)
                s[i] = rnd_char("aB0P9y ,_");
    } else {
        shape = (int)(gen_seed() % 4);
        if (shape <= 1) {
            hide(s, n, (int)rnd(n / 2, n), alnum, seps, shape == 1 ? 2 : 0);
        } else if (shape == 2) {
            for (i = 0; i < n; i++)
                s[i] = rnd_char(seps);
        } else {
            for (i = 0; i < n; i++)
                s[i] = (char)rnd(32, 126);
        }
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
