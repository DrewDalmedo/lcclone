/* n coupons. Small cases: a few coupons with short codes over a, A, 0 and
 * _ (so equal codes, prefixes and case differences come up), now and then
 * an empty code or one bad character, lines of business that are mostly
 * right and otherwise near misses, and most coupons switched on. Full
 * size: 100 coupons with codes and wrong lines of business up to 100
 * characters, bad characters drawn from all of printable ASCII. */
static const char *lines[] = {"electronics", "grocery", "pharmacy",
                              "restaurant"};
static const char *nearMiss[] = {"Grocery", "pharmacy ", "electronic", "",
                                 "restaurants", "PHARMACY", " grocery",
                                 "electronicsx", "food"};

static int isGood(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || c == '_';
}

/* A printable ASCII character that may not be in a code. */
static char badChar(void)
{
    char c;

    do
        c = (char)rnd(32, 126);
    while (isGood(c));
    return c;
}

static char *makeCode(int small)
{
    static const char good[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
    int len, i, pos;
    char *s;

    if (rnd_chance(10))
        len = 0;
    else
        len = small ? (int)rnd(1, 3)
                    : (rnd_chance(20) ? 100 : (int)rnd(1, 100));
    s = malloc((size_t)len + 1);
    for (i = 0; i < len; i++)
        s[i] = small ? rnd_char("aA0_") : rnd_char(good);
    if (len > 0 && rnd_chance(small ? 15 : 25)) {
        pos = (int)rnd(0, len - 1);
        s[pos] = small ? rnd_char(" -.@") : badChar();
    }
    s[len] = '\0';
    return s;
}

static char *makeLine(int small)
{
    const char *base;
    char *s;
    int len, extra, i;

    if (rnd_chance(70)) {
        base = lines[rnd(0, 3)];
        s = malloc(strlen(base) + 1);
        strcpy(s, base);
        return s;
    }
    if (small || rnd_chance(50)) {
        base = nearMiss[rnd(0, 8)];
        s = malloc(strlen(base) + 1);
        strcpy(s, base);
        return s;
    }
    /* A right name followed by more characters, up to 100 in all, or
     * random printable characters. */
    s = malloc(101);
    if (rnd_chance(50)) {
        base = lines[rnd(0, 3)];
        len = (int)strlen(base);
        memcpy(s, base, (size_t)len);
        extra = (int)rnd(1, 100 - len);
        for (i = 0; i < extra; i++)
            s[len++] = (char)rnd(32, 126);
    } else {
        len = (int)rnd(0, 100);
        for (i = 0; i < len; i++)
            s[i] = (char)rnd(32, 126);
    }
    s[len] = '\0';
    return s;
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, i;
    char *code[100], *line[100];
    bool active[100];

    for (i = 0; i < n; i++) {
        code[i] = makeCode(small);
        line[i] = makeLine(small);
        active[i] = rnd_chance(80);
    }
    put_strings(code, n);
    put_strings(line, n);
    put_bools(active, n);
    for (i = 0; i < n; i++) {
        free(code[i]);
        free(line[i]);
    }
}
