/* A random digit string and block size. Small cases are short, with a
 * small k and digits from a palette the seed picks (all zeros, all nines,
 * zeros and nines, ...), so that zero totals, two-digit totals and passes
 * that keep the length come up often; now and then s is a long run of
 * nines with k from 14 to 30, whose totals pass 127 and 255. Full-size
 * ones have 100 digits, by a shape the seed picks: random digits and k;
 * all nines with k = 2 (the most passes); all nines with a large k; or
 * mostly zeros with a small k. */
void gen(int small)
{
    static const char *palette[] = {
        "0123456789", "0123456789", "09", "0", "9", "01", "789", "0009"
    };
    char s[101];
    const char *chars;
    int n, k, shape, i;

    if (small) {
        shape = rnd_chance(90) ? 0 : 1;
        if (shape == 0) {
            n = (int)rnd(1, 14);
            k = rnd_chance(80) ? (int)rnd(2, 5) : (int)rnd(2, 16);
            chars = palette[rnd(0, 7)];
        } else {
            k = (int)rnd(14, 30);
            n = (int)rnd(k + 1, 2 * k + 2);
            chars = "99999999998";
        }
    } else {
        shape = (int)rnd(0, 3);
        n = 100;
        if (shape == 0) {
            k = (int)rnd(2, 99);
            chars = "0123456789";
        } else if (shape == 1) {
            k = 2;
            chars = "9";
        } else if (shape == 2) {
            k = (int)rnd(14, 99);
            chars = "9";
        } else {
            k = (int)rnd(2, 6);
            chars = "000000000123456789";
        }
    }
    for (i = 0; i < n; i++)
        s[i] = rnd_char(chars);
    s[n] = '\0';
    put_string(s);
    put_int(k);
}
