/* Small cases have 1 to 8 letters drawn from the first 2 to 5 letters of
 * the alphabet, so that repeats are common. Full-size cases have 100
 * letters, in a shape the seed picks: random over all 26; random over 3;
 * a cycle of 3 to 26 letters (every place good); one letter repeated (none
 * good); two letters alternating (none good, though neighbours always
 * differ); or random over 4 letters with no two neighbours equal. */
void gen(int small)
{
    char s[101], x, y;
    int n = small ? (int)rnd(1, 8) : 100, i, k, base, shape;

    s[n] = '\0';
    if (small) {
        k = (int)rnd(2, 5);
        for (i = 0; i < n; i++)
            s[i] = (char)('a' + rnd(0, k - 1));
        put_string(s);
        return;
    }
    shape = (int)(gen_seed() % 6);
    if (shape == 0) {
        for (i = 0; i < n; i++)
            s[i] = (char)('a' + rnd(0, 25));
    } else if (shape == 1) {
        base = (int)rnd(0, 23);
        for (i = 0; i < n; i++)
            s[i] = (char)('a' + base + rnd(0, 2));
    } else if (shape == 2) {
        k = (int)rnd(3, 26);
        base = (int)rnd(0, 26 - k);
        for (i = 0; i < n; i++)
            s[i] = (char)('a' + base + i % k);
    } else if (shape == 3) {
        x = (char)('a' + rnd(0, 25));
        for (i = 0; i < n; i++)
            s[i] = x;
    } else if (shape == 4) {
        x = (char)('a' + rnd(0, 25));
        y = (char)('a' + (x - 'a' + rnd(1, 25)) % 26);
        for (i = 0; i < n; i++)
            s[i] = i % 2 == 0 ? x : y;
    } else {
        base = (int)rnd(0, 22);
        for (i = 0; i < n; i++) {
            do
                s[i] = (char)('a' + base + rnd(0, 3));
            while (i > 0 && s[i] == s[i - 1]);
        }
    }
    put_string(s);
}
