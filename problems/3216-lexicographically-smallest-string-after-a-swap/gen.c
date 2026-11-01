/* A string of digits, in one of three shapes picked by the seed: random
 * digits (from a small pool in small cases, so equal digits and pairs of
 * the same parity come up often); digits alternating in parity, so that no
 * exchange is allowed at all; or alternating digits whose only allowed and
 * useful exchange is the last pair. Full-size cases are up to 100 digits
 * long, often exactly 100. */
static char parity_digit(int p)
{
    return p ? rnd_char("13579") : rnd_char("02468");
}

void gen(int small)
{
    static const char *pools[] = {"012", "0246", "1357", "0123456789"};
    char s[101];
    const char *pool;
    int n, i, shape, p, hi, lo;

    if (small)
        n = (int)rnd(2, 8);
    else
        n = rnd_chance(50) ? 100 : (int)rnd(2, 100);
    shape = (int)rnd(0, 2);
    if (shape == 0) {
        pool = small ? pools[rnd(0, 3)] : "0123456789";
        for (i = 0; i < n; i++)
            s[i] = rnd_char(pool);
    } else {
        p = (int)rnd(0, 1);
        for (i = 0; i < n; i++)
            s[i] = parity_digit((p + i) % 2);
        if (shape == 2) {
            /* Same parity as s[n - 2], at least 2, and a smaller digit of
             * that parity after it. */
            p = (s[n - 2] - '0') % 2;
            hi = p + 2 * (int)rnd(1, 4);
            lo = p + 2 * (int)rnd(0, (hi - p) / 2 - 1);
            s[n - 2] = (char)('0' + hi);
            s[n - 1] = (char)('0' + lo);
        }
    }
    s[n] = '\0';
    put_string(s);
}
