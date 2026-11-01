/* A column number, made by drawing a name and working out its number.
 * Small cases draw names of one to three letters, mostly from A, B, Y and
 * Z, so that multiples of 26 and the ends of each length come up often.
 * Full-size ones draw names of six or seven letters (Z-heavy or uniform),
 * or a number close to 2^31 - 1; a name whose number would pass 2^31 - 1
 * is drawn again. */
void gen(int small)
{
    long long n;
    int len, i, shape;

    if (!small && rnd_chance(20)) {
        put_int(2147483647LL - rnd(0, 1000));
        return;
    }
    shape = (int)rnd(0, 2);
    do {
        len = small ? (int)rnd(1, 3) : (int)rnd(6, 7);
        n = 0;
        for (i = 0; i < len; i++) {
            char c;

            if (shape == 0)
                c = rnd_char("ABYZ");
            else if (shape == 1)
                c = rnd_chance(60) ? 'Z' : rnd_char("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            else
                c = rnd_char("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            n = n * 26 + (c - 'A' + 1);
        }
    } while (n > 2147483647LL);
    put_int(n);
}
