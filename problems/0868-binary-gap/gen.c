/* Small cases are numbers up to 600, or a few 1s at places 0 to 9. Full-size
 * cases, in a shape the seed picks, are any number up to 10^9; two to five
 * 1s at random places up to 29; a power of two; or two 1s, one of them at
 * place 0 or 1 and the other at place 27 to 29, for the widest distances. */
static long long bits_at(int count, int top)
{
    long long v = 0;
    int i, place;

    for (i = 0; i < count; i++) {
        place = (int)rnd(0, top);
        v |= 1LL << place;
    }
    return v;
}

void gen(int small)
{
    long long n, hi, lo;
    int shape;

    if (small) {
        shape = (int)rnd(0, 3);
        if (shape <= 1)
            n = rnd(1, 600);
        else if (shape == 2)
            n = bits_at((int)rnd(1, 4), 9);
        else
            n = 1LL << rnd(0, 9);
    } else {
        shape = (int)(gen_seed() % 4);
        do {
            if (shape == 0) {
                n = rnd(1, 1000000000);
            } else if (shape == 1) {
                n = bits_at((int)rnd(2, 5), 29);
            } else if (shape == 2) {
                n = 1LL << rnd(0, 29);
            } else {
                hi = rnd(27, 29);
                lo = rnd(0, 1);
                n = (1LL << hi) | (1LL << lo);
            }
        } while (n > 1000000000);
    }
    put_int(n);
}
