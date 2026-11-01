/* A random block, its two columns and its two rows each drawn on their
 * own and put in order. Small cases often make a block one column wide or
 * one row tall, or put an edge on column A or Z or row 1 or 9. Full-size
 * cases are wide and tall, up to the whole sheet A1:Z9. */
static void order(int *lo, int *hi)
{
    int t;

    if (*lo > *hi) {
        t = *lo;
        *lo = *hi;
        *hi = t;
    }
}

void gen(int small)
{
    char s[6];
    int c1, c2, r1, r2;

    if (small) {
        c1 = (int)rnd(0, 25);
        c2 = rnd_chance(25) ? c1 : (int)rnd(0, 25);
        order(&c1, &c2);
        if (rnd_chance(15))
            c1 = 0;
        if (rnd_chance(15))
            c2 = 25;
        r1 = (int)rnd(1, 9);
        r2 = rnd_chance(25) ? r1 : (int)rnd(1, 9);
        order(&r1, &r2);
        if (rnd_chance(15))
            r1 = 1;
        if (rnd_chance(15))
            r2 = 9;
    } else {
        c1 = (int)rnd(0, 3);
        c2 = (int)rnd(22, 25);
        r1 = (int)rnd(1, 2);
        r2 = (int)rnd(8, 9);
    }
    s[0] = (char)('A' + c1);
    s[1] = (char)('0' + r1);
    s[2] = ':';
    s[3] = (char)('A' + c2);
    s[4] = (char)('0' + r2);
    s[5] = '\0';
    put_string(s);
}
