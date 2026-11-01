/* A string of letters. Small cases have up to 10 letters from two or three
 * letters in both cases (aAbB, aAbBcC), so clashes, cascades, same-case
 * doubles and overlaps like aAa come up often. Full-size ones have 100
 * letters, in a shape the seed picks: a nest of 50 letters followed by its
 * mirror in the other cases, which all erases one pair at a time from the
 * middle (the slowest shape for searching again from the start); such a
 * nest with a few letters left over; random letters from aAbB; or random
 * letters from the whole alphabet. */
static char flip(char c)
{
    return islower((unsigned char)c) ? (char)toupper((unsigned char)c)
                                     : (char)tolower((unsigned char)c);
}

static const char *letters =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void gen(int small)
{
    char s[101];
    int n, shape, i, half, at;
    char c;

    if (small) {
        n = (int)rnd(1, 10);
        shape = (int)rnd(0, 1);
        for (i = 0; i < n; i++)
            s[i] = rnd_char(shape ? "aAbB" : "aAbBcC");
    } else {
        n = 100;
        shape = (int)(gen_seed() % 4);
        if (shape <= 1) {
            half = n / 2;
            for (i = 0; i < half; i++)
                s[i] = rnd_char(letters);
            for (i = 0; i < half; i++)
                s[n - 1 - i] = flip(s[i]);
            if (shape == 1)
                for (i = 0; i < 3; i++) {
                    at = (int)rnd(0, n - 1);
                    c = rnd_char(letters);
                    s[at] = c;
                }
        } else {
            for (i = 0; i < n; i++)
                s[i] = rnd_char(shape == 2 ? "aAbB" : letters);
        }
    }
    s[n] = '\0';
    put_string(s);
}
