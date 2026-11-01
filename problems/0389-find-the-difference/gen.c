/* t is s with one more letter put in at a random place, and then, more
 * often than not, shuffled. Small cases: s has up to 7 letters (sometimes
 * none) from a tiny alphabet, so the extra letter is usually one that s
 * has already. Full-size cases: s has 1000 letters, in a shape the seed
 * picks: random letters, shuffled; random letters with the extra one at
 * the end of t, not shuffled; one letter throughout, the extra one too; or
 * a and b only, shuffled. */
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

void gen(int small)
{
    static const char *alphabets[] = {
        "a", "ab", "abc", "az", "xyz", "abcdefghijklmnopqrstuvwxyz"
    };
    const char *abc;
    int n, pos, mix, i, shape = 0;
    char s[1001], t[1002], extra;

    if (small) {
        abc = alphabets[rnd(0, 5)];
        n = (int)rnd(0, 7);
        mix = rnd_chance(60);
    } else {
        shape = (int)(gen_seed() % 4);
        abc = shape == 2 ? alphabets[rnd(0, 5)] : shape == 3 ? "ab"
              : alphabets[5];
        n = 1000;
        mix = shape != 1;
    }
    if (shape == 2) {
        char c = rnd_char(abc);

        memset(s, c, (size_t)n);
        extra = c;
    } else {
        for (i = 0; i < n; i++)
            s[i] = rnd_char(abc);
        extra = rnd_char(abc);
    }
    pos = shape == 1 ? n : (int)rnd(0, n);
    memcpy(t, s, (size_t)pos);
    t[pos] = extra;
    memcpy(t + pos + 1, s + pos, (size_t)(n - pos));
    if (mix)
        shuffle(t, n + 1);
    s[n] = '\0';
    t[n + 1] = '\0';
    put_string(s);
    put_string(t);
}
