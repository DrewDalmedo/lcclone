/* name is built as runs of one letter each, and typed gives every run a
 * run at least as long, so typed starts out as a stretch of name; then a
 * change may spoil it. Small cases use up to 4 short runs of a, b and c
 * (neighbouring runs may share a letter), and half the time one change:
 * a character of typed deleted, changed or inserted, one added at its end,
 * or a character of name deleted. Full-size cases, in a shape the seed
 * picks: a true case with typed exactly 1000 long; name = k copies of one
 * letter and another letter, typed = 999 copies of the first and a third
 * letter (slow for a search that tries every block length); runs of 250
 * to 300 letters with the last run of typed one short, or not; or a long
 * stretch whose very last character is changed. Long stretches have
 * either a mix of short and very long runs, or over 300 runs of 1 or 2. */
static char name[1002], typed[1002];
static int nlen, tlen;
static char rc[1000];
static int rk[1000];

static void add(char *s, int *len, char c, int k)
{
    while (k-- > 0)
        s[(*len)++] = c;
}

static char other_than(char a, char b, const char *pool)
{
    char d;

    do
        d = rnd_char(pool);
    while (d == a || d == b);
    return d;
}

/* Drops s[p] from s, which has *len characters. */
static void drop(char *s, int *len, int p)
{
    memmove(s + p, s + p + 1, (size_t)(*len - p - 1));
    (*len)--;
}

static void small_case(void)
{
    int runs = (int)rnd(1, 4), r, k, extra, p, kind;
    char c;

    for (r = 0; r < runs; r++) {
        c = rnd_char("abc");
        k = (int)rnd(1, 3);
        extra = rnd_chance(50) ? (int)rnd(1, 2) : 0;
        add(name, &nlen, c, k);
        add(typed, &tlen, c, k + extra);
    }
    if (!rnd_chance(50))
        return;
    kind = (int)rnd(0, 4);
    if (kind == 0 && tlen > 1) {
        p = (int)rnd(0, tlen - 1);
        drop(typed, &tlen, p);
    } else if (kind == 1) {
        p = (int)rnd(0, tlen - 1);
        typed[p] = other_than(typed[p], typed[p], "abc");
    } else if (kind == 2) {
        p = (int)rnd(0, tlen);
        memmove(typed + p + 1, typed + p, (size_t)(tlen - p));
        typed[p] = rnd_char("abc");
        tlen++;
    } else if (kind == 3) {
        typed[tlen++] = rnd_char("abc");
    } else if (nlen > 1) {
        p = (int)rnd(0, nlen - 1);
        drop(name, &nlen, p);
    }
}

/* Runs of letters from "abcde" until name holds about 500 characters,
 * then typed gets the same runs, with the 1000 - nlen extra copies
 * spread over them at random. Half the time every run is 1 or 2 long and
 * differs from the one before it, so name has over 300 runs. */
static void stretch_case(void)
{
    int runs = 0, r, k, extra, many = rnd_chance(50);

    while (nlen < 500) {
        if (many)
            k = (int)rnd(1, 2);
        else
            k = rnd_chance(10) ? (int)rnd(1, 200) : (int)rnd(1, 5);
        if (k > 500 - nlen)
            k = 500 - nlen;
        if (many && runs > 0)
            rc[runs] = other_than(rc[runs - 1], rc[runs - 1], "abcde");
        else
            rc[runs] = rnd_char("abcde");
        rk[runs++] = k;
        nlen += k;
    }
    nlen = 0;
    for (r = 0; r < runs; r++)
        add(name, &nlen, rc[r], rk[r]);
    for (extra = 1000 - nlen; extra > 0; extra--) {
        r = (int)rnd(0, runs - 1);
        rk[r]++;
    }
    for (r = 0; r < runs; r++)
        add(typed, &tlen, rc[r], rk[r]);
}

void gen(int small)
{
    static const char alpha[] = "abcdefghijklmnopqrstuvwxyz";
    int shape, k, r, len;
    char x, y, z;

    nlen = tlen = 0;
    if (small) {
        small_case();
    } else {
        shape = (int)(gen_seed() % 4);
        if (shape == 0 || shape == 3) {
            stretch_case();
            if (shape == 3)
                typed[tlen - 1] = other_than(typed[tlen - 1],
                                             typed[tlen - 1], alpha);
        } else if (shape == 1) {
            k = (int)rnd(400, 600);
            x = rnd_char(alpha);
            y = other_than(x, x, alpha);
            z = other_than(x, y, alpha);
            add(name, &nlen, x, k);
            add(name, &nlen, y, 1);
            add(typed, &tlen, x, 999);
            add(typed, &tlen, z, 1);
        } else {
            x = 'a';
            for (r = 0; r < 3; r++) {
                y = x;
                x = other_than(y, y, alpha);
                len = (int)rnd(250, 300);
                k = (int)rnd(0, 10);
                add(name, &nlen, x, len);
                if (r == 2 && rnd_chance(50))
                    k = -1;
                add(typed, &tlen, x, len + k);
            }
        }
    }
    name[nlen] = '\0';
    typed[tlen] = '\0';
    put_string(name);
    put_string(typed);
}
