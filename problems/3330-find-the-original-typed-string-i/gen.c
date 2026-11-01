/* Small cases have 1 to 8 letters from "ab" or "abc", so that blocks of
 * equal letters, and the same letter in separate blocks, come up often.
 * Full-size cases have 100 letters, with the seed picking the shape:
 * letters from a small alphabet; one letter throughout; no two neighbours
 * equal; or a few long blocks. */
void gen(int small)
{
    char s[101];
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = small ? 0 : (int)rnd(0, 3);
    const char *alpha;
    int i, len, c;

    if (small || shape == 0) {
        c = (int)rnd(0, 1);
        alpha = c ? "abc" : "ab";
        for (i = 0; i < n; i++)
            s[i] = rnd_char(alpha);
    } else if (shape == 1) {
        c = rnd_char("abcdefghijklmnopqrstuvwxyz");
        for (i = 0; i < n; i++)
            s[i] = (char)c;
    } else if (shape == 2) {
        s[0] = rnd_char("abcdefghijklmnopqrstuvwxyz");
        for (i = 1; i < n; i++)
            do
                s[i] = rnd_char("abcdefghijklmnopqrstuvwxyz");
            while (s[i] == s[i - 1]);
    } else {
        i = 0;
        while (i < n) {
            len = (int)rnd(1, 30);
            c = rnd_char("xyz");
            while (len-- > 0 && i < n)
                s[i++] = (char)c;
        }
    }
    s[n] = '\0';
    put_string(s);
}
