/* A string of n characters: m bracket pairs mixed at random with n - 2m
 * digits and signs, kept in no particular order. The seed picks how the
 * brackets nest: a walk that opens and closes about equally often; one
 * that mostly opens (deep); one that mostly closes (many shallow pairs);
 * all pairs nested inside each other; or no brackets at all. Small cases
 * have 1 to 12 characters; full-size ones 100, with up to 50 pairs, the
 * deepest nesting the length allows, so that the deep shapes pass what a
 * 32-entry stack holds. */

/* Writes 2m brackets: '(' with a chance of pct in 100 while one may still
 * be opened, ')' otherwise; the count of open brackets never goes below 0.
 * pct = 100 nests all pairs inside each other. */
static void walk(char *b, int m, int pct)
{
    int opens = m, depth = 0, i;

    for (i = 0; i < 2 * m; i++) {
        if (opens > 0 && (depth == 0 || rnd_chance(pct))) {
            b[i] = '(';
            opens--;
            depth++;
        } else {
            b[i] = ')';
            depth--;
        }
    }
}

void gen(int small)
{
    static const int pcts[] = {50, 85, 25, 100};
    char s[101], b[100];
    int n = small ? (int)rnd(1, 12) : 100;
    int shape = (int)rnd(0, 4), m, others, nb = 0, i, pick;

    if (shape == 4)
        m = 0;
    else if (small)
        m = (int)rnd(1, n / 2 > 0 ? n / 2 : 1);
    else
        m = (int)rnd(shape == 2 ? 20 : 33, 50);
    if (2 * m > n)
        m = 0;
    if (m > 0)
        walk(b, m, pcts[shape]);
    others = n - 2 * m;
    for (i = 0; i < n; i++) {
        pick = (int)rnd(1, n - i);
        if (pick <= others) {
            s[i] = rnd_chance(60) ? rnd_char("0123456789") : rnd_char("+-*/");
            others--;
        } else {
            s[i] = b[nb++];
        }
    }
    s[n] = '\0';
    put_string(s);
}
