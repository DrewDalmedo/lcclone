/* Strings of lowercase letters. Plain random strings over a few letters
 * nearly always contain a doubled letter or a swapped link, so the seed
 * often builds a string with none instead: m letters (m odd) in a ring,
 * each next letter 1 to (m - 1) / 2 places further round the ring. Such a
 * string never has a link and its swap, though letters repeat, at least
 * three apart. Sometimes one link is then swapped in somewhere else, or a
 * letter doubled, which makes the answer true. Small cases have 1 to 8
 * letters; full-size cases 100. */
void gen(int small)
{
    char s[101];
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = (int)rnd(0, 3);
    int m, base, step, cur, i, j, k;

    if (shape == 0) {
        m = small ? (int)rnd(2, 4) : (int)rnd(2, 26);
        base = (int)rnd(0, 26 - m);
        for (i = 0; i < n; i++) {
            k = (int)rnd(0, m - 1);
            s[i] = (char)('a' + base + k);
        }
    } else {
        m = 2 * (int)rnd(1, small ? 3 : 12) + 1;
        base = (int)rnd(0, 26 - m);
        cur = (int)rnd(0, m - 1);
        for (i = 0; i < n; i++) {
            s[i] = (char)('a' + base + cur);
            step = (int)rnd(1, (m - 1) / 2);
            cur = (cur + step) % m;
        }
        if (shape == 2 && n >= 2) {
            /* Copy the link at i, swapped, to j; or double a letter. */
            i = (int)rnd(0, n - 2);
            j = (int)rnd(0, n - 2);
            if (j == i) {
                s[i + 1] = s[i];
            } else {
                k = s[i];
                s[j] = s[i + 1];
                s[j + 1] = (char)k;
            }
        }
    }
    s[n] = '\0';
    put_string(s);
}
