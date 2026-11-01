/* A number from 0 to 2^31 - 1. Small cases pick a digit count from 1 to
 * 10 and make each digit after the first a 0 with a chance of 40 in 100,
 * so that groups like 000, 005 and 040 come up often; now and then they
 * take an edge instead: 0, 2^31 - 1, a power of ten or one less. Full-size
 * ones are 10-digit numbers made the same way, or one of those edges. */
void gen(int small)
{
    static const long long edges[] = {
        0, 2147483647, 1000, 999, 1000000, 999999, 1000000000, 999999999,
        2000000000, 2147483000
    };
    int digits, i, d;
    long long v;

    if (rnd_chance(20)) {
        i = (int)rnd(0, 9);
        put_int(edges[i]);
        return;
    }
    digits = small ? (int)rnd(1, 10) : 10;
    do {
        v = rnd(digits == 1 ? 0 : 1, digits == 10 ? 2 : 9);
        for (i = 1; i < digits; i++) {
            d = rnd_chance(40) ? 0 : (int)rnd(0, 9);
            v = v * 10 + d;
        }
    } while (v > 2147483647LL);
    put_int(v);
}
