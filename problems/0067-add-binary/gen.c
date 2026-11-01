/* Two binary numbers. Each is "0", nothing but 1s (long carry chains),
 * or random bits after a leading 1, and the two lengths are picked
 * separately, so they usually differ. Small cases have up to 8 bits;
 * full-size ones have up to 10^4, and often exactly 10^4, both 1s
 * throughout (a carry through every column), or a lone "1" or "0" against
 * a number of 10^4 bits. */
static void number(int small, int len)
{
    char *s = malloc((size_t)len + 1);
    int shape = (int)rnd(0, 9), i;

    if (len == 1 && shape < 4) {
        s[0] = '0';
    } else {
        s[0] = '1';
        for (i = 1; i < len; i++)
            s[i] = shape <= 3 ? '1' : rnd_char(small ? "01" : "0111");
    }
    s[len] = '\0';
    put_string(s);
    free(s);
}

static int length(int small)
{
    if (small)
        return (int)rnd(1, 8);
    switch (rnd(0, 3)) {
    case 0:
        return 1;
    case 1:
        return (int)rnd(1, 10000);
    default:
        return 10000;
    }
}

void gen(int small)
{
    int la = length(small), lb = length(small);

    number(small, la);
    number(small, lb);
}
