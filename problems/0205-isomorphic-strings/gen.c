/* Small cases draw s and t from tiny sets of characters (letters, quotes
 * and backslashes, tabs, newlines and DEL), so that repeats are common:
 * t is either s put through a random one-to-one code, sometimes with one
 * character spoiled, or random. Full-size cases are 5 * 10^4 characters
 * with codes from 1 to 127, in a shape the seed picks: s coded through a
 * random permutation of all 127 codes; s from two characters, coded; or a
 * code that breaks only at the last position, either because a character
 * of s seen before gets a new code character there, or because a new
 * character of s gets a code character already taken. */
static const char *const pools[] = {
    "ab", "abc", "xyz", "a1 ", "\"\\'", "\t\n\x7f", "\x01~a",
};

/* code[c] for every c in 1..127: a random permutation of 1..127. */
static void permutation(int *code)
{
    int vals[127], i;

    for (i = 0; i < 127; i++)
        vals[i] = i + 1;
    rnd_shuffle(vals, 127);
    for (i = 0; i < 127; i++)
        code[i + 1] = vals[i];
}

static void small_case(char *s, char *t, int n)
{
    const char *from = pools[rnd(0, 6)], *to = pools[rnd(0, 6)];
    char code[128] = {0}, used[128] = {0}, c;
    int i, shape = (int)rnd(0, 9);

    for (i = 0; i < n; i++)
        s[i] = rnd_char(from);
    if (shape < 6) {
        /* Code characters come mostly from `to`, else from a to j. */
        for (i = 0; i < n; i++) {
            unsigned char a = (unsigned char)s[i];

            if (!code[a]) {
                do {
                    c = rnd_chance(80) ? rnd_char(to) : rnd_char("abcdefghij");
                } while (used[(unsigned char)c]);
                code[a] = c;
                used[(unsigned char)c] = 1;
            }
            t[i] = code[a];
        }
        if (shape >= 4) {
            c = rnd_char(to);
            t[rnd(0, n - 1)] = c;
        }
    } else {
        for (i = 0; i < n; i++)
            t[i] = rnd_char(to);
    }
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 50000, i, shape;
    int code[128];
    char *s = malloc((size_t)n + 1), *t = malloc((size_t)n + 1);

    if (small) {
        small_case(s, t, n);
    } else {
        shape = (int)(gen_seed() % 4);
        permutation(code);
        for (i = 0; i < n; i++)
            s[i] = (char)(shape == 1 ? rnd(1, 2) * 50 : rnd(1, 126));
        if (shape == 0)
            s[n - 1] = 127;
        for (i = 0; i < n; i++)
            t[i] = (char)code[(unsigned char)s[i]];
        if (shape == 2) {
            /* s[0] again, but coded as 127's code, which is not in use. */
            s[n - 1] = s[0];
            t[n - 1] = (char)code[127];
        } else if (shape == 3) {
            /* 127, new to s, coded as s[0] is. */
            s[n - 1] = 127;
            t[n - 1] = (char)code[(unsigned char)s[0]];
        }
    }
    s[n] = '\0';
    t[n] = '\0';
    put_string(s);
    put_string(t);
    free(s);
    free(t);
}
