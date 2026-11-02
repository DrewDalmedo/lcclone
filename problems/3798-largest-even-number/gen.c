/* A row of 1s and 2s, shaped by the seed: no 2 at all, a single 2
 * (often at the front), a row that ends in 2, or 2s scattered at a random
 * density. Small cases have up to 10 digits; full-size ones have 100. */
void gen(int small)
{
    char s[101];
    int n, shape, pct, pos, i;

    n = small ? (int)rnd(1, 10) : 100;
    shape = (int)rnd(0, 3);
    pct = small ? (int)rnd(20, 80) : (int)rnd(5, 60);
    for (i = 0; i < n; i++)
        s[i] = rnd_chance(pct) ? '2' : '1';
    if (shape == 0) {
        memset(s, '1', (size_t)n);
    } else if (shape == 1) {
        memset(s, '1', (size_t)n);
        pos = rnd_chance(25) ? 0 : (int)rnd(0, n - 1);
        s[pos] = '2';
    } else if (shape == 2) {
        s[n - 1] = '2';
    }
    s[n] = '\0';
    put_string(s);
}
