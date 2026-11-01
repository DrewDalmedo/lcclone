/* A random separator, and strings of letters, separators and the other
 * punctuation marks. Small cases: up to 5 strings of up to 6 characters
 * over a, b, the separator (often, so that runs of it come up) and now
 * and then another mark; some strings, and now and then all of them, are
 * separators only. Full size: mostly 100 strings, each by the seed's
 * shape one that alternates a character and a separator (10 chunks, the
 * most a string can have), one of 20 characters with no separator, or a
 * random mix at a random density of separators. */
static const char marks[] = ".,|$#@";
static const char alpha[] = "abcdefghijklmnopqrstuvwxyz";

static char one_char(int small, int kind, int j, int start, int dens,
                     char sep, const char *others)
{
    int r;

    if (small) {
        if (kind)
            return sep;
        r = (int)rnd(0, 99);
        if (r < 40)
            return sep;
        return r < 85 ? rnd_char("ab") : rnd_char(others);
    }
    if (kind == 0 && j % 2 == start)
        return sep;
    if (kind <= 1)
        return rnd_chance(15) ? rnd_char(others) : rnd_char(alpha);
    r = (int)rnd(0, 99);
    if (r < dens)
        return sep;
    return r < dens + 10 ? rnd_char(others) : rnd_char(alpha);
}

void gen(int small)
{
    static char words[100][21];
    char *ptrs[100], others[6];
    char sep = rnd_char(marks);
    int n, shape, dens, all_sep, kind, start, len, i, j, o = 0;

    for (i = 0; i < 6; i++)
        if (marks[i] != sep)
            others[o++] = marks[i];
    others[o] = '\0';
    if (small) {
        n = (int)rnd(1, 5);
        all_sep = rnd_chance(8);
        shape = dens = 0;
    } else {
        n = rnd_chance(80) ? 100 : (int)rnd(1, 100);
        all_sep = 0;
        shape = (int)rnd(0, 3);
        dens = (int)rnd(5, 60);
    }
    for (i = 0; i < n; i++) {
        if (small) {
            kind = all_sep || rnd_chance(10);
            len = (int)rnd(1, 6);
        } else {
            kind = shape == 3 ? (int)rnd(0, 2) : shape;
            len = kind == 2 ? (int)rnd(1, 20) : 20;
        }
        start = (int)rnd(0, 1);
        for (j = 0; j < len; j++)
            words[i][j] = one_char(small, kind, j, start, dens, sep, others);
        words[i][len] = '\0';
        ptrs[i] = words[i];
    }
    put_strings(ptrs, n);
    put_char(sep);
}
