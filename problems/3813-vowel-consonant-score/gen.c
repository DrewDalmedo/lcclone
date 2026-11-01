/* A mix of vowels, consonants ('y' often among them), digits and spaces,
 * shaped by the seed: random weights for the four kinds, no consonant, no
 * letter at all, or nearly all vowels with one or two consonants (large
 * scores). Small cases have up to 12 characters; full-size ones have 100. */
static char pick(int kind, int small)
{
    switch (kind) {
    case 0:
        return rnd_char("aeiou");
    case 1:
        return rnd_char(small ? "bcy" : "bcdfghjklmnpqrstvwxyz");
    case 2:
        return rnd_char(small ? "07" : "0123456789");
    default:
        return ' ';
    }
}

void gen(int small)
{
    char s[101], ch;
    int n, shape, w[4], total, i, r, kind, extra, pos;

    n = small ? (int)rnd(1, 12) : 100;
    shape = (int)rnd(0, 5);
    w[0] = (int)rnd(0, 12);
    w[1] = (int)rnd(0, 6);
    w[2] = (int)rnd(0, 3);
    w[3] = (int)rnd(0, 3);
    if (shape == 0)
        w[1] = 0;
    else if (shape == 1)
        w[0] = w[1] = 0;
    else if (shape == 2) {
        w[0] = 30;
        w[1] = 0;
    }
    total = w[0] + w[1] + w[2] + w[3];
    if (total == 0) {
        w[3] = 1;
        total = 1;
    }
    for (i = 0; i < n; i++) {
        r = (int)rnd(0, total - 1);
        for (kind = 0; r >= w[kind]; kind++)
            r -= w[kind];
        s[i] = pick(kind, small);
    }
    if (shape == 2) {
        /* one or two consonants among the vowels */
        extra = rnd_chance(70) ? 1 : 2;
        for (i = 0; i < extra; i++) {
            pos = (int)rnd(0, n - 1);
            ch = pick(1, small);
            s[pos] = ch;
        }
    }
    s[n] = '\0';
    put_string(s);
}
