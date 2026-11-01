/* Codes of five kinds: digits with leading zeros, all zeros, letters
 * only, digits with letters mixed in, and full-length numbers. Small
 * cases use a few codes over the letters "ab" and mostly short numbers,
 * so that a number ties or loses to a length often. Full-size ones have
 * 100 codes; the seed picks whether big numbers may appear at all
 * (without them, every number is a single digit after its zeros, so the
 * answer often comes from a length). */
static void code(char *s, int kind, int digit_top, int sig_top)
{
    int len = (int)rnd(1, 9), sig, i, at;

    if (kind == 0) {                /* zero-padded number */
        sig = (int)rnd(1, len < sig_top ? len : sig_top);
        for (i = 0; i < len - sig; i++)
            s[i] = '0';
        for (; i < len; i++)
            s[i] = (char)('0' + rnd(i == len - sig ? 1 : 0, digit_top));
    } else if (kind == 1) {         /* all zeros */
        for (i = 0; i < len; i++)
            s[i] = '0';
    } else if (kind == 2) {         /* letters only */
        for (i = 0; i < len; i++)
            s[i] = rnd_char("abz");
    } else if (kind == 3) {         /* digits with one or more letters */
        for (i = 0; i < len; i++)
            s[i] = (char)('0' + rnd(0, 9));
        at = (int)rnd(0, len - 1);
        s[at] = rnd_char("abz");
        if (rnd_chance(30)) {
            at = (int)rnd(0, len - 1);
            s[at] = rnd_char("xy");
        }
    } else {                        /* any number, up to nine digits */
        for (i = 0; i < len; i++)
            s[i] = (char)('0' + rnd(0, digit_top));
    }
    s[len] = '\0';
}

void gen(int small)
{
    static char buf[100][10];
    char *strs[100];
    int n = small ? (int)rnd(1, 6) : 100, big = 1, kind, i;

    if (!small)
        big = rnd_chance(50);
    for (i = 0; i < n; i++) {
        kind = (int)rnd(0, big ? 4 : 3);
        if (small && kind == 4 && rnd_chance(70))
            kind = 0;
        code(buf[i], kind, small ? 3 : 9, big ? 2 : 1);
        strs[i] = buf[i];
    }
    /* Full size with big numbers: the largest possible number, last. */
    if (!small && big && rnd_chance(50))
        strcpy(buf[n - 1], "999999999");
    put_strings(strs, n);
}
