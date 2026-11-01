/* Lowercase strings. Small cases have 1 to 8 letters from two or three
 * different ones, so matches are common. Full-size ones have 99 or 100
 * letters and the seed picks the shape: no matching pair at all (so the
 * answer is -1 for even n and the middle for odd n), only the innermost
 * pair matching, or one matching pair at a random place. */
void gen(int small)
{
    char s[101], c;
    int n, shape, i, k;

    if (small) {
        const char *letters = rnd_chance(50) ? "ab" : "xyz";
        n = (int)rnd(1, 8);
        for (i = 0; i < n; i++)
            s[i] = rnd_char(letters);
    } else {
        n = (int)rnd(99, 100);
        shape = (int)rnd(0, 2);
        /* Left half from a..m, right half from n..z: nothing matches. */
        for (i = 0; i < n; i++)
            s[i] = rnd_char(i < n / 2 ? "abcdefghijklm" : "nopqrstuvwxyz");
        if (shape == 1) {
            k = (n - 1) / 2 - (n % 2);
            if (k >= 0)
                s[n - 1 - k] = s[k];
        } else if (shape == 2) {
            k = (int)rnd(0, n / 2 - 1);
            c = s[k];
            s[n - 1 - k] = c;
        }
    }
    s[n] = '\0';
    put_string(s);
}
