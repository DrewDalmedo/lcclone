/* The seed picks the shape. Either the string starts with m different
 * letters (from a shuffled alphabet), then repeats one of them, then goes
 * on with random letters; in full-size cases m is often 26, which puts the
 * answer as far right as it can be. Or every letter is drawn from a few
 * letters (small cases) or from the whole alphabet; if that leaves no
 * repeat, the last letter is changed into an earlier one. Small cases have
 * 2 to 10 letters, full-size ones 100. The alphabet is sometimes taken from
 * its start or end, so that a and z come up. */
void gen(int small)
{
    char s[101];
    int alpha[26], n = small ? (int)rnd(2, 10) : 100;
    int shape = (int)rnd(0, 1), k, lo, m, seen = 0, dup = 0, i, j;

    for (i = 0; i < 26; i++)
        alpha[i] = i;
    rnd_shuffle(alpha, 26);
    if (shape == 0) {
        m = small ? (int)rnd(1, n - 1) : rnd_chance(50) ? 26 : (int)rnd(1, 26);
        for (i = 0; i < m; i++)
            s[i] = (char)('a' + alpha[i]);
        j = (int)rnd(0, m - 1);
        s[m] = s[j];
        for (i = m + 1; i < n; i++) {
            j = (int)rnd(0, 25);
            s[i] = (char)('a' + j);
        }
    } else {
        k = small ? (int)rnd(1, 5) : (int)rnd(1, 26);
        j = (int)rnd(0, 2);
        lo = j == 0 ? 0 : j == 1 ? 26 - k : (int)rnd(0, 26 - k);
        for (i = 0; i < n; i++) {
            j = (int)rnd(0, k - 1);
            s[i] = (char)('a' + lo + j);
            if (seen & (1 << (s[i] - 'a')))
                dup = 1;
            seen |= 1 << (s[i] - 'a');
        }
        if (!dup) {
            j = (int)rnd(0, n - 2);
            s[n - 1] = s[j];
        }
    }
    s[n] = '\0';
    put_string(s);
}
