/* Small cases are up to 10 letters from a tiny set, often one letter in
 * both cases, so that pairs, leftovers and near misses such as a and A come
 * up often. Full-size ones are 2000 letters (1999 or 2000 for a single
 * letter), in a shape the seed picks: random over all 52 letters; every
 * letter an odd number of times; every letter an even number of times; a
 * single letter repeated; or one letter in both cases. */
static const char letters[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

static void shuffle(char *a, int n)
{
    int i, k;
    char c;

    for (i = n - 1; i > 0; i--) {
        k = (int)rnd(0, i);
        c = a[i];
        a[i] = a[k];
        a[k] = c;
    }
}

/* Gives each of the 52 letters `start` tiles, then adds `pairs` pairs of
 * tiles to random letters, and writes them all to s in a random order. */
static int from_counts(char *s, int start, int pairs)
{
    int count[52], n = 0, i, k;

    for (k = 0; k < 52; k++)
        count[k] = start;
    for (i = 0; i < pairs; i++) {
        k = (int)rnd(0, 51);
        count[k] += 2;
    }
    for (k = 0; k < 52; k++)
        for (i = 0; i < count[k]; i++)
            s[n++] = letters[k];
    shuffle(s, n);
    return n;
}

void gen(int small)
{
    static const char *sets[] = {"aab", "ab", "aAb", "qQq", "abcde",
                                 "zZyY", "AuzZ", "xXyz"};
    const char *set;
    char s[2001], c;
    int n, i, k, shape;

    if (small) {
        set = sets[rnd(0, 7)];
        n = (int)rnd(1, 10);
        for (i = 0; i < n; i++)
            s[i] = rnd_char(set);
    } else {
        n = 2000;
        shape = (int)(gen_seed() % 5);
        if (shape == 0) {
            for (i = 0; i < n; i++)
                s[i] = rnd_char(letters);
        } else if (shape == 1) {
            n = from_counts(s, 1, (n - 52) / 2);
        } else if (shape == 2) {
            n = from_counts(s, 0, n / 2);
        } else if (shape == 3) {
            n = (int)rnd(1999, 2000);
            c = rnd_char(letters);
            memset(s, c, (size_t)n);
        } else {
            k = (int)rnd(0, 25);
            for (i = 0; i < n; i++)
                s[i] = rnd_chance(50) ? letters[k] : letters[k + 26];
        }
    }
    s[n] = '\0';
    put_string(s);
}
