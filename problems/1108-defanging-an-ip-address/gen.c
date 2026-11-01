/* A random address. In small cases each field first picks how many digits
 * it has (1, 2 or 3), then a value of that many digits with no leading
 * zero, so short and long fields mix; 0, 9, 10, 99, 100 and 255 come up
 * often. Full-size cases give every field three digits, for the longest
 * address, 15 characters. */
static int field(int digits)
{
    static const int lo[] = {0, 0, 10, 100}, hi[] = {0, 9, 99, 255};
    int edge = (int)rnd(0, 3);

    if (edge == 0)
        return lo[digits];
    if (edge == 1)
        return hi[digits];
    return (int)rnd(lo[digits], hi[digits]);
}

void gen(int small)
{
    char s[16];
    int len = 0, i, digits, v;

    for (i = 0; i < 4; i++) {
        digits = small ? (int)rnd(1, 3) : 3;
        v = field(digits);
        if (i > 0)
            s[len++] = '.';
        len += sprintf(s + len, "%d", v);
    }
    s[len] = '\0';
    put_string(s);
}
