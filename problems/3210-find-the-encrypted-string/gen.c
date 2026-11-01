/* A string and a step count k. Small cases use a few letters of a tiny
 * alphabet (so letters repeat) and k near n, a multiple of n, or anywhere
 * up to 10^4; full-size ones use up to 100 letters of a to z and k up to
 * 10^4, sometimes exactly 10^4 or a multiple of n. */
void gen(int small)
{
    char s[101];
    const char *letters;
    int n, k, i, shape;

    if (small) {
        n = (int)rnd(1, 8);
        letters = rnd_chance(50) ? "ab" : "abcde";
    } else {
        n = rnd_chance(50) ? 100 : (int)rnd(1, 100);
        letters = "abcdefghijklmnopqrstuvwxyz";
    }
    for (i = 0; i < n; i++)
        s[i] = rnd_char(letters);
    s[n] = '\0';

    shape = (int)rnd(0, 3);
    if (shape == 0)
        k = (int)rnd(1, small ? 3 * n : 10000);
    else if (shape == 1)
        k = n * (int)rnd(1, 10000 / n);
    else if (shape == 2)
        k = small ? (int)rnd(1, 10000) : 10000;
    else
        k = (int)rnd(10000 - 2 * n, 10000);
    put_string(s);
    put_int(k);
}
