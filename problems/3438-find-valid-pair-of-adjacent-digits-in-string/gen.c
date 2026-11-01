/* Small cases: some of the digits 1 to 4 (now and then a 5 or 6), each
 * with as many copies as its value or one more or fewer, laid out in runs
 * or shuffled, so that complete digits, equal neighbours and near misses
 * are common. Full-size ones are picked by the seed: 100 random digits;
 * 100 digits whose only good pair is the last one; 100 digits where the
 * complete digits stand in runs fenced by another digit, so that nothing
 * is good; or all nine digits complete (45 characters) in a random order. */
static void shuffle(char *s, int n)
{
    int i, j;
    char t;

    for (i = n - 1; i > 0; i--) {
        j = (int)rnd(0, i);
        t = s[i];
        s[i] = s[j];
        s[j] = t;
    }
}

static int put_copies(char *s, int n, int d, int c)
{
    while (c-- > 0)
        s[n++] = (char)('0' + d);
    return n;
}

static void small_case(char *s, int *len)
{
    int order[6] = {1, 2, 3, 4, 5, 6}, top = (int)rnd(2, 6), n = 0, i, c;

    rnd_shuffle(order, top);
    for (i = 0; i < top; i++) {
        if (order[i] > 4 && !rnd_chance(30))
            continue;
        if (rnd_chance(20))
            continue;
        c = order[i];
        if (rnd_chance(30))
            c += rnd_chance(50) ? 1 : -1;
        n = put_copies(s, n, order[i], c);
    }
    while (n < 2) {
        c = (int)rnd(1, 9);
        s[n++] = (char)('0' + c);
    }
    if (rnd_chance(50))
        shuffle(s, n);
    *len = n;
}

void gen(int small)
{
    char s[101];
    int digits[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9}, slots[48];
    int n = 0, shape, a, b, f, i, c, extra;

    if (small) {
        small_case(s, &n);
        s[n] = '\0';
        put_string(s);
        return;
    }
    shape = (int)rnd(0, 3);
    rnd_shuffle(digits, 9);
    if (shape == 0) {
        for (n = 0; n < 100; n++) {
            c = (int)rnd(1, 9);
            s[n] = (char)('0' + c);
        }
    } else if (shape == 1) {
        /* Filler f everywhere, the spare copies of a and b on even indices
         * up to 94 (never next to each other), and a, b at 98 and 99. */
        a = digits[0];
        b = digits[1];
        f = digits[2];
        for (n = 0; n < 100; n++)
            s[n] = (char)('0' + f);
        for (i = 0; i < 48; i++)
            slots[i] = 2 * i;
        rnd_shuffle(slots, 48);
        extra = 0;
        for (i = 0; i < a - 1; i++)
            s[slots[extra++]] = (char)('0' + a);
        for (i = 0; i < b - 1; i++)
            s[slots[extra++]] = (char)('0' + b);
        s[98] = (char)('0' + a);
        s[99] = (char)('0' + b);
    } else if (shape == 2) {
        /* Runs of complete digits, each followed by one to three copies
         * of the filler digits[0], which ends up far too common. */
        f = digits[0];
        for (i = 1; i < 9; i++) {
            if (rnd_chance(40))
                continue;
            n = put_copies(s, n, digits[i], digits[i]);
            c = (int)rnd(1, 3);
            n = put_copies(s, n, f, c);
        }
        n = put_copies(s, n, f, 100 - n);
    } else {
        for (i = 1; i <= 9; i++)
            n = put_copies(s, n, i, i);
        shuffle(s, n);
    }
    s[n] = '\0';
    put_string(s);
}
