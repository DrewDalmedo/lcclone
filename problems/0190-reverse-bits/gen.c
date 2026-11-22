/* One unsigned 32-bit number, in a pattern the seed picks: random bits;
 * the highest and lowest bits set and others between them (few for small
 * cases, half for full-size ones); a mirror image of itself; or, for
 * full-size cases only, a single run of ones. */
void gen(int small)
{
    unsigned long long n = 0;
    int i, lo, hi;

    switch (small ? (int)rnd(0, 2) : (int)(gen_seed() % 4)) {
    case 0:
        n = rnd(0, 4294967295LL);
        break;
    case 1:
        for (i = 0; i < 32; i++)
            if (rnd_chance(small ? 10 : 50))
                n |= 1ULL << i;
        n |= 1ULL | 1ULL << 31;
        break;
    case 2:
        for (i = 0; i < 16; i++)
            if (rnd_chance(50))
                n |= 1ULL << i | 1ULL << (31 - i);
        break;
    default:
        lo = (int)rnd(0, 31);
        hi = (int)rnd(lo, 31);
        for (i = lo; i <= hi; i++)
            n |= 1ULL << i;
        break;
    }
    put_int((long long)n);
}
