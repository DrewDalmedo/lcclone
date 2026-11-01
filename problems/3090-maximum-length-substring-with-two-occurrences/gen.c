/* The seed picks a shape: letters from a small alphabet (third copies
 * come up often), letters from the whole alphabet, an alphabet cycled in
 * a shuffled order (each letter about four times in 100, so the answer
 * is capped at 52), or a prefix of repeated a's and b's followed by
 * letters used at most twice each, so the best piece tends to be a
 * suffix. Small cases have 2 to 10 letters, full-size ones 100. */
void gen(int small)
{
    char s[101];
    int bag[52], order[26];
    int n = small ? (int)rnd(2, 10) : 100, shape = (int)rnd(0, 3);
    int m, p, i;

    if (shape == 0) {
        m = (int)rnd(1, small ? 4 : 6);
        for (i = 0; i < n; i++)
            s[i] = (char)('a' + rnd(0, m - 1));
    } else if (shape == 1) {
        for (i = 0; i < n; i++)
            s[i] = (char)('a' + rnd(0, 25));
    } else if (shape == 2) {
        m = small ? (int)rnd(2, 4) : 26;
        for (i = 0; i < m; i++)
            order[i] = i;
        rnd_shuffle(order, m);
        for (i = 0; i < n; i++)
            s[i] = (char)('a' + order[i % m]);
    } else {
        m = small ? 6 : 26;
        p = small ? (int)rnd(0, n / 2) : n - (int)rnd(30, 52);
        for (i = 0; i < p; i++)
            s[i] = rnd_char("ab");
        for (i = 0; i < 2 * m; i++)
            bag[i] = i % m;
        rnd_shuffle(bag, 2 * m);
        for (i = p; i < n; i++)
            s[i] = (char)('a' + bag[i - p]);
    }
    s[n] = '\0';
    put_string(s);
}
