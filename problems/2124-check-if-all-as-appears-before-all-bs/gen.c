/* Small cases have 1 to 8 letters, full-size ones 100. Starts from k a's
 * followed by b's (k = 0 or the whole length often, for one-letter
 * strings), then, as the seed picks: leaves it alone, swaps the pair where
 * the a's meet the b's into "ba" (anywhere from the first pair to the
 * last), flips one letter, or draws every letter at random. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = (int)rnd(0, 3), k, i, p;
    char s[101];

    if (rnd_chance(25))
        k = rnd_chance(50) ? 0 : n;
    else
        k = (int)rnd(0, n);
    for (i = 0; i < n; i++)
        s[i] = i < k ? 'a' : 'b';
    if (shape == 1 && k > 0 && k < n) {
        s[k - 1] = 'b';
        s[k] = 'a';
    } else if (shape == 2) {
        p = (int)rnd(0, n - 1);
        s[p] = s[p] == 'a' ? 'b' : 'a';
    } else if (shape == 3) {
        for (i = 0; i < n; i++)
            s[i] = rnd_char("ab");
    }
    s[n] = '\0';
    put_string(s);
}
