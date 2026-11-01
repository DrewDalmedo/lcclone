/* A string of stars, bars and letters with an even number of bars. The
 * seed picks the mix: stars, bars and letters at random; mostly bars, so
 * that empty stretches and tiny regions come up often; no bars at all
 * (sometimes stars only, the largest answer); or stars everywhere with a
 * few bars, sometimes one pair at both ends. Small cases have 1 to 12
 * characters and letters a and b, and are mostly of the first two mixes;
 * full-size ones have 1000 characters. An odd bar left over at the end is
 * turned into a star. */
void gen(int small)
{
    static char s[1001];
    int n = small ? (int)rnd(1, 12) : 1000;
    static const int small_shapes[] = {0, 0, 0, 0, 1, 1, 1, 2, 3, 3};
    int shape = small ? small_shapes[rnd(0, 9)] : (int)rnd(0, 3);
    int bar_pct = small ? 35 : (int)rnd(1, 40);
    int all_stars = rnd_chance(30), both_ends = rnd_chance(50);
    const char *letters = small ? "ab" : "abcdefghijklmnopqrstuvwxyz";
    int bars = 0, i, k, r, p, q;

    for (i = 0; i < n; i++) {
        r = (int)rnd(0, 99);
        if (shape == 0)
            s[i] = r < bar_pct ? '|' : r < bar_pct + 40 ? '*' : rnd_char(letters);
        else if (shape == 1)
            s[i] = r < 50 ? '|' : r < 90 ? '*' : rnd_char(letters);
        else if (shape == 2 && all_stars)
            s[i] = '*';
        else
            s[i] = r < 85 ? '*' : rnd_char(letters);
    }
    if (shape == 3 && n >= 2) {
        k = small ? 1 : (int)rnd(1, 3);
        for (i = 0; i < k; i++) {
            if (both_ends && i == 0) {
                p = 0;
                q = n - 1;
            } else {
                p = (int)rnd(0, n - 1);
                q = (int)rnd(0, n - 1);
            }
            s[p] = '|';
            s[q] = '|';
        }
    }
    for (i = 0; i < n; i++)
        bars += s[i] == '|';
    if (bars % 2 == 1) {
        k = (int)rnd(1, bars);
        for (i = 0; i < n; i++)
            if (s[i] == '|' && --k == 0) {
                s[i] = '*';
                break;
            }
    }
    s[n] = '\0';
    put_string(s);
}
