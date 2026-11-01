/* Small cases draw each kind of character from 3 of its characters,
 * picked anew for each case, so that repeats are likely and every symbol
 * (- and ^ among them) turns up on its own over many cases; full-size
 * cases use every character and are 100 long. The seed picks the shape:
 *   0. a password that passes every check (8 to 12 long when small);
 *   1. one that passes the other checks but is 1 to 7 long (small only;
 *      full size makes shape 3 with its repeat in the last two places);
 *   2. one with no character of one kind, and no other fault;
 *   3. one whose only fault is a single pair of equal neighbours, often
 *      in the first or last two places;
 *   4. characters of any kind, at random, with no repair. */
static const char *const kind_chars[4] = {
    "abcdefghijklmnopqrstuvwxyz",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    "0123456789",
    "!@#$%^&*()-+",
};

static char pool[4][27];
static int pool_size[4];

static void make_pools(int small)
{
    int k, i, j, len, dup;
    char c;

    for (k = 0; k < 4; k++) {
        len = (int)strlen(kind_chars[k]);
        if (!small) {
            memcpy(pool[k], kind_chars[k], (size_t)len + 1);
            pool_size[k] = len;
            continue;
        }
        for (i = 0; i < 3;) {
            c = kind_chars[k][rnd(0, len - 1)];
            dup = 0;
            for (j = 0; j < i; j++)
                if (pool[k][j] == c)
                    dup = 1;
            if (!dup)
                pool[k][i++] = c;
        }
        pool[k][3] = '\0';
        pool_size[k] = 3;
    }
}

static char from_kind(int k)
{
    return pool[k][rnd(0, pool_size[k] - 1)];
}

/* Fills s[0..len) with characters of the kinds in `allowed` (a bit per
 * kind), each allowed kind at least once when len leaves room, and no
 * two equal neighbours. */
static void build(char *s, int len, int allowed)
{
    int kind_at[100], kinds[4], nk = 0, k, i;
    char c;

    for (k = 0; k < 4; k++)
        if (allowed & (1 << k))
            kinds[nk++] = k;
    for (i = 0; i < len; i++)
        kind_at[i] = i < nk ? kinds[i] : kinds[rnd(0, nk - 1)];
    rnd_shuffle(kind_at, len);
    for (i = 0; i < len; i++) {
        do
            c = from_kind(kind_at[i]);
        while (i > 0 && c == s[i - 1]);
        s[i] = c;
    }
    s[len] = '\0';
}

void gen(int small)
{
    char s[101];
    int shape = (int)rnd(0, 4), len, p, i, kind;

    make_pools(small);
    if (shape == 1 && !small)
        shape = 3;
    if (shape == 0) {
        len = small ? (int)rnd(8, 12) : 100;
        build(s, len, 15);
    } else if (shape == 1) {
        len = (int)rnd(1, 7);
        build(s, len, 15);
    } else if (shape == 2) {
        len = small ? (int)rnd(8, 12) : 100;
        kind = (int)rnd(0, 3);
        build(s, len, 15 & ~(1 << kind));
    } else if (shape == 3) {
        len = small ? (int)rnd(9, 12) : 100;
        build(s, len - 1, 15);
        p = (int)rnd(0, 3);
        if (p == 0 || (!small && p == 1))
            p = len - 2;
        else if (p == 1)
            p = 0;
        else
            p = (int)rnd(0, len - 2);
        for (i = len - 1; i > p; i--)
            s[i] = s[i - 1];
        s[len] = '\0';
    } else {
        len = small ? (int)rnd(1, 12) : 100;
        for (i = 0; i < len; i++) {
            kind = (int)rnd(0, 3);
            s[i] = from_kind(kind);
        }
        s[len] = '\0';
    }
    put_string(s);
}
