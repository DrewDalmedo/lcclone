/* Strings built run by run. Small cases: a few short runs over two or three
 * letters, so that equal letters meet across runs and ties are common.
 * Full-size ones have 500 letters, picked by the seed: a single run; runs
 * of random lengths whose strictly longest comes last; no two neighbours
 * equal; or random runs up to 300 long, past what a byte can count. */
static char other(const char *letters, char prev)
{
    char c;

    do
        c = rnd_char(letters);
    while (c == prev);
    return c;
}

void gen(int small)
{
    static const char *az = "abcdefghijklmnopqrstuvwxyz";
    char s[501];
    int n = small ? (int)rnd(1, 12) : 500;
    int shape = small ? -1 : (int)rnd(0, 3);
    int len = 0, run, cap = 1, tail = 0, i;
    const char *letters = az;
    char c = 0;

    if (small)
        letters = rnd_chance(50) ? "ab" : "abc";
    if (shape == 0) {
        c = rnd_char(az);
        for (len = 0; len < n; len++)
            s[len] = c;
    } else if (shape == 2) {
        for (len = 0; len < n; len++) {
            c = other(az, c);
            s[len] = c;
        }
    } else {
        if (small)
            cap = 4;
        else if (shape == 1)
            cap = (int)rnd(2, 60);
        else
            cap = (int)rnd(2, 300);
        if (shape == 1)
            tail = (int)rnd(cap + 1, cap + 100);
        while (len < n - tail) {
            run = (int)rnd(1, cap);
            if (run > n - tail - len)
                run = n - tail - len;
            c = shape == 1 ? other(letters, c) : rnd_char(letters);
            for (i = 0; i < run; i++)
                s[len++] = c;
        }
        if (tail > 0) {
            c = other(letters, c);
            while (len < n)
                s[len++] = c;
        }
    }
    s[len] = '\0';
    put_string(s);
}
