/* Digits drawn from a three-digit alphabet, so that the digit a recoding
 * picks often comes back later in num, and often behind leading 9s.
 * Small cases have 1 to 6 digits; full-size ones 8, in a shape the seed
 * picks: any 8-digit number; leading 9s, then digits from the alphabet;
 * one digit repeated; or 10^8, the only 9-digit num. */
static int build(int len, int nines)
{
    int alpha[3], v = 0, i, d, pick;

    alpha[0] = rnd_chance(50) ? 0 : 9;
    alpha[1] = (int)rnd(0, 9);
    alpha[2] = (int)rnd(1, 8);
    for (i = 0; i < len; i++) {
        if (i < nines) {
            d = 9;
        } else {
            pick = (int)rnd(0, 2);
            d = alpha[pick];
            if (i == 0 && d == 0)
                d = alpha[2];
        }
        v = v * 10 + d;
    }
    return v;
}

void gen(int small)
{
    int shape = (int)rnd(0, 3), len, d, i, v = 0, nines;

    if (small) {
        len = rnd_chance(10) ? 1 : (int)rnd(2, 6);
        nines = rnd_chance(30) ? (int)rnd(1, len) : 0;
        put_int(build(len, nines));
        return;
    }
    if (shape == 0) {
        put_int(rnd(10000000, 99999999));
    } else if (shape == 1) {
        nines = (int)rnd(1, 7);
        put_int(build(8, nines));
    } else if (shape == 2) {
        d = (int)rnd(1, 9);
        for (i = 0; i < 8; i++)
            v = v * 10 + d;
        put_int(v);
    } else {
        put_int(100000000);
    }
}
