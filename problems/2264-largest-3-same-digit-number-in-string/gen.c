/* Digit strings built from runs of one repeated digit. Small cases join
 * runs of 1 to 4 copies of two or three digits (0 among them half the
 * time), so that pairs, triples, longer runs and "000" all come up often.
 * Full-size cases have 1000 digits in one of four shapes, picked by the
 * seed: no triple anywhere (runs of 1 or 2); runs of 1 or 2 with a single
 * triple at the very end; random runs of 1 to 5 over all ten digits; or
 * one digit throughout. */

/* A digit of `digits` other than `avoid`. */
static char pick(const char *digits, char avoid)
{
    char c;

    do
        c = rnd_char(digits);
    while (c == avoid);
    return c;
}

void gen(int small)
{
    static const char *sets[] = {"09", "05", "019", "078", "12", "345"};
    int n = small ? (int)rnd(3, 12) : 1000;
    int shape = small ? 4 : (int)rnd(0, 3);
    const char *digits = "0123456789";
    char s[1001], c = 0;
    int len = 0, run;

    if (small)
        digits = sets[rnd(0, 5)];
    if (shape == 1)
        n -= 3;
    while (len < n) {
        if (shape == 0 || shape == 1) {
            c = pick(digits, c);
            run = (int)rnd(1, 2);
        } else if (shape == 2) {
            c = rnd_char(digits);
            run = (int)rnd(1, 5);
        } else if (shape == 3) {
            c = rnd_char(digits);
            run = n;
        } else {
            c = rnd_char(digits);
            run = (int)rnd(1, 4);
        }
        if (run > n - len)
            run = n - len;
        memset(s + len, c, (size_t)run);
        len += run;
    }
    if (shape == 1) {
        /* A triple of a digit other than the last one, so that it is
         * exactly three long. */
        c = pick(digits, c);
        memset(s + len, c, 3);
        len += 3;
    }
    s[len] = '\0';
    put_string(s);
}
