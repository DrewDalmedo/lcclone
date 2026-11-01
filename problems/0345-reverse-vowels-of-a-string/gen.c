/* Small cases draw up to 8 characters from a tiny pool (vowels of both
 * cases, y and Y, a space, quotes and backslashes, a few consonants), or
 * from all printable characters, so that strings with no vowel, one
 * vowel, only vowels and repeated vowels come up often. Full-size cases
 * are 3 * 10^5 characters, in a shape the seed picks: any printable
 * characters; only vowels; no vowel but the first and last characters;
 * vowels only in the middle third; or no vowel at all. */
static const char vowels[] = "aeiouAEIOU";

static char other(void)
{
    char c;

    do
        c = (char)rnd(32, 126);
    while (strchr(vowels, c));
    return c;
}

void gen(int small)
{
    static const char *const pools[] = {
        "aeiouAEIOU", "aEobx", "oOuyY", "ua i", "I\"\\eA.", "uUzEk", "aeiyO",
    };
    int n = small ? (int)rnd(1, 8) : 300000, i, shape;
    char *s = malloc((size_t)n + 1);

    if (small) {
        shape = (int)rnd(0, 7);
        for (i = 0; i < n; i++)
            s[i] = shape < 7 ? rnd_char(pools[shape]) : (char)rnd(32, 126);
    } else {
        shape = (int)(gen_seed() % 5);
        for (i = 0; i < n; i++) {
            if (shape == 0)
                s[i] = (char)rnd(32, 126);
            else if (shape == 1 || (shape == 3 && i >= n / 3 && i < 2 * n / 3
                                    && rnd_chance(50)))
                s[i] = rnd_char(vowels);
            else
                s[i] = other();
        }
        if (shape == 2) {
            s[0] = rnd_char(vowels);
            s[n - 1] = rnd_char(vowels);
        }
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
