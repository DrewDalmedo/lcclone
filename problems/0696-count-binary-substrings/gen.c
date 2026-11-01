/* A string of 0s and 1s. Small cases have up to 14 digits, built either
 * as runs of 1 to 4 equal digits (so that neighbouring runs tie, nest and
 * differ by one) or digit by digit, with a chance of a 1 the seed picks.
 * Full-size ones have 10^5 digits, in a shape the seed picks: all one
 * digit (the slowest input for walking each run again from every start);
 * two runs of 50000; alternating (10^5 runs, the largest answer);
 * random runs of up to 1000 digits; or random digits. */
static void runs(char *s, int n, int longest)
{
    int i = 0, len, k;
    char d = rnd_char("01");

    while (i < n) {
        len = (int)rnd(1, longest);
        for (k = 0; k < len && i < n; k++)
            s[i++] = d;
        d = d == '0' ? '1' : '0';
    }
}

void gen(int small)
{
    static const int chance[] = {10, 30, 50, 70, 90};
    int n = small ? (int)rnd(1, 14) : 100000, ones, i;
    char *s = malloc((size_t)n + 1);
    char d;

    if (small) {
        if (rnd_chance(60)) {
            runs(s, n, 4);
        } else {
            ones = chance[rnd(0, 4)];
            for (i = 0; i < n; i++)
                s[i] = rnd_chance(ones) ? '1' : '0';
        }
    } else {
        d = rnd_char("01");
        switch (gen_seed() % 5) {
        case 0: /* all one digit */
            for (i = 0; i < n; i++)
                s[i] = d;
            break;
        case 1: /* two runs of 50000 */
            for (i = 0; i < n; i++)
                s[i] = i < n / 2 ? d : (char)('0' + '1' - d);
            break;
        case 2: /* alternating */
            for (i = 0; i < n; i++)
                s[i] = i % 2 ? (char)('0' + '1' - d) : d;
            break;
        case 3: /* long random runs */
            runs(s, n, 1000);
            break;
        default: /* random digits */
            for (i = 0; i < n; i++)
                s[i] = rnd_char("01");
            break;
        }
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
