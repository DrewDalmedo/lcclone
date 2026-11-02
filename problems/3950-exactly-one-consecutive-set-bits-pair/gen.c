/* Random numbers alone rarely have exactly one pair, so most cases are
 * built: a run of 1 to 5 one bits (2, the true case, half the time) at a
 * random place, plus lone 1 bits that touch no other 1, and in one shape a
 * last 1 bit set anywhere, which may lengthen a run or add a second pair.
 * Small cases use bits 0 to 7 (or any n up to 63); full-size ones bits 0
 * to 16 within 10^5, or n near 10^5, the seed picking the shape. */
void gen(int small)
{
    int top = small ? 7 : 16, shape, r, p, tries, i, b;
    long long limit = small ? 255 : 100000, n, near;

    shape = small ? (int)rnd(0, 2) : (int)(gen_seed() % 4);
    if (shape == 0) {
        n = small ? rnd(0, 63) : rnd(0, 100000);
        put_int(n);
        return;
    }
    if (shape == 3) {
        put_int(rnd(99000, 100000));
        return;
    }
    do {
        r = rnd_chance(50) ? 2 : (int)rnd(1, 5);
        p = (int)rnd(0, top + 1 - r);
        n = ((1LL << r) - 1) << p;
        tries = (int)rnd(0, top);
        for (i = 0; i < tries; i++) {
            b = (int)rnd(0, top);
            near = (7LL << b) >> 1;
            if ((n & near) == 0)
                n |= 1LL << b;
        }
        if (shape == 2) {
            b = (int)rnd(0, top);
            n |= 1LL << b;
        }
    } while (n > limit);
    put_int(n);
}
