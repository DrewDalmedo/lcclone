/* Three numbers, each with 1 to 4 digits (four most often), so that the
 * padding zeros come up. Small cases draw the digits after the first from
 * 0, 1 and 9 only, so that zeros and ties are common; full-size ones from
 * all ten digits. The first digit is never 0. */
static int number(int small)
{
    int digits = rnd_chance(50) ? 4 : (int)rnd(1, 3), v, i, d;

    v = (int)rnd(1, 9);
    for (i = 1; i < digits; i++) {
        d = small ? rnd_char("019") - '0' : (int)rnd(0, 9);
        v = v * 10 + d;
    }
    return v;
}

void gen(int small)
{
    int i;

    for (i = 0; i < 3; i++)
        put_int(number(small));
}
