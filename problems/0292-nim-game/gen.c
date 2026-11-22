/* One pile size. Small cases are mostly 1 to 40, so every remainder mod 4
 * comes up often, and otherwise up to 5000 (brute.c plays every pile up to
 * n, which is quick at that size). Full-size ones are all at least
 * 2^31 - 2^28, so that playing out every pile up to n takes nearly two
 * billion steps, in a shape the seed picks: one of the three largest ints
 * (none a multiple of 4), a multiple of 4, any other int, or 2^31 - 2^28
 * itself. */
void gen(int small)
{
    long long n;

    if (small) {
        n = rnd_chance(80) ? rnd(1, 40) : rnd(41, 5000);
    } else {
        switch ((int)(gen_seed() % 4)) {
        case 0:
            n = 2147483647LL - rnd(0, 2);
            break;
        case 1:
            n = 4 * rnd(469762048LL, 536870911LL);
            break;
        case 2:
            n = 4 * rnd(469762048LL, 536870911LL);
            n += rnd(1, 3);
            break;
        default:
            n = (1LL << 31) - (1LL << 28);
        }
    }
    put_int(n);
}
