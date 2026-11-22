/* Two binary numbers. Each is "0", nothing but 1s (long carry chains),
 * or random bits after a leading 1, and the two lengths are picked
 * separately, so they usually differ. Small cases have up to 8 bits.
 * Full-size ones have up to 10^4, in a shape the seed picks: both 10^4
 * 1s (a carry through every column); 10^4 1s and "1" (one carry that runs
 * through them all); "0" and a number of 10^4 random bits; or two numbers
 * made as for small cases, often of exactly 10^4 bits. */
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

/* Prints len 1s, or "0" when len is 0. */
static void ones(int len)
{
    char *s = malloc((size_t)len + 2);

    if (len == 0) {
        strcpy(s, "0");
    } else {
        memset(s, '1', (size_t)len);
        s[len] = '\0';
    }
    put_string(s);
    free(s);
}

void gen(int small)
{
    int la, lb, shape = small ? 3 : (int)(gen_seed() % 4);

    if (shape == 0) {
        ones(10000);
        ones(10000);
    } else if (shape == 1) {
        ones(10000);
        ones(1);
    } else if (shape == 2) {
        ones(0);
        number(small, 10000);
    } else {
        la = length(small);
        lb = length(small);
        number(small, la);
        number(small, lb);
    }
}
