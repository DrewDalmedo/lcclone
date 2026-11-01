/* A random string and a random k. The seed picks the letters: a, b only
 * (so prefixes are often palindromes, or the letters at the cut equal),
 * a, b, c, or the whole alphabet. Small cases are up to 8 letters long;
 * full-size ones 100, with k = 100, k = 99 or k anywhere from 1 to 100. */
void gen(int small)
{
    static const char *sets[] = {"ab", "abc", "abcdefghijklmnopqrstuvwxyz"};
    int n = small ? (int)rnd(1, 8) : 100, i, k, which, shape;
    char s[101];

    which = (int)rnd(0, 2);
    for (i = 0; i < n; i++)
        s[i] = rnd_char(sets[which]);
    s[n] = '\0';
    k = (int)rnd(1, n);
    if (!small) {
        shape = (int)rnd(0, 3);
        if (shape == 0)
            k = n;
        else if (shape == 1)
            k = n - 1;
    }
    put_string(s);
    put_int(k);
}
