/* One pile size. Small cases are mostly 1 to 40, so every remainder mod 4
 * comes up often, and otherwise up to 5000 (brute.c plays every pile up to
 * n, which is quick at that size). Full-size ones take a shape the seed
 * picks: one of the eight largest ints (2^31 - 8 to 2^31 - 1, with 2^31 - 4
 * and 2^31 - 8 the multiples of 4), any positive int, a multiple of 4
 * anywhere in range (half the time plus 1 to 3), or a power of two from
 * 2^0 to 2^30. */
void gen(int small)
{
    long long n;

    if (small) {
        n = rnd_chance(80) ? rnd(1, 40) : rnd(41, 5000);
    } else {
        switch ((int)(gen_seed() % 4)) {
        case 0:
            n = 2147483647LL - rnd(0, 7);
            break;
        case 1:
            n = rnd(1, 2147483647LL);
            break;
        case 2:
            n = 4 * rnd(1, 536870911LL);
            if (rnd_chance(50))
                n += rnd(1, 3);
            break;
        default:
            n = 1LL << rnd(0, 30);
        }
    }
    put_int(n);
}
