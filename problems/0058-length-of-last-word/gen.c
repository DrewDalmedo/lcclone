/* Letters and spaces. Small cases are a few characters from "aBc" and
 * many spaces, so that leading, doubled and trailing spaces come up
 * often. Full-size ones are 10^4 characters, in a shape the seed picks:
 * one word with no space at all, one letter followed by nothing but
 * spaces, random words and gaps, or text, a long gap and a long last
 * word. */
static const char letters[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* Fills s[from..to) with random words of up to maxw letters and gaps of
 * up to maxgap spaces. */
static void words(char *s, int from, int to, int maxw, int maxgap)
{
    int i = from, k, len;

    while (i < to) {
        len = (int)rnd(1, maxw);
        for (k = 0; k < len && i < to; k++)
            s[i++] = rnd_char(letters);
        len = (int)rnd(1, maxgap);
        for (k = 0; k < len && i < to; k++)
            s[i++] = ' ';
    }
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 12) : 10000, i, cut;
    char *s = malloc((size_t)n + 1);

    if (small) {
        for (i = 0; i < n; i++)
            s[i] = rnd_chance(40) ? ' ' : rnd_char("aBc");
        for (i = 0; i < n && s[i] == ' '; i++)
            ;
        if (i == n) {
            char c = rnd_char("aBc");

            s[rnd(0, n - 1)] = c;
        }
    } else {
        switch (gen_seed() % 4) {
        case 0: /* a single word of 10^4 letters */
            for (i = 0; i < n; i++)
                s[i] = rnd_char(letters);
            break;
        case 1: /* the only word is the first letter */
            s[0] = rnd_char(letters);
            for (i = 1; i < n; i++)
                s[i] = ' ';
            break;
        case 2: /* random words and gaps, maybe with spaces at both ends */
            words(s, 0, n, 12, 5);
            cut = (int)rnd(0, 20);
            for (i = 0; i < cut; i++)
                s[i] = ' ';
            s[cut] = rnd_char(letters);
            cut = (int)rnd(0, 20);
            for (i = n - cut; i < n; i++)
                s[i] = ' ';
            break;
        default: /* text, a long gap, then a long last word at the end */
            cut = (int)rnd(1000, 3000);
            words(s, 0, cut, 8, 3);
            for (i = cut; i < 5000; i++)
                s[i] = ' ';
            for (i = 5000; i < n; i++)
                s[i] = rnd_char(letters);
            break;
        }
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
