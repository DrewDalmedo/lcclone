/* One box: three sides and a mass. The seed picks how the sides come near
 * a boundary: a volume one step of the third side above, at or below 10^9
 * with every side below 10^4 (two sides often divide 10^9, so the volume
 * can be exactly 10^9); one side near 10^4 with the other two short; or
 * each side from a list of telling values, such as 999, 1000, 1001, 2048
 * (whose products overflow 32 bits), 9999, 10^4 and 10^5. Full-size cases
 * may also take every side anywhere in range. The mass is often 1, 99,
 * 100, 101 or 1000, and otherwise anything from 1 to 1000. */
void gen(int small)
{
    static const int divisors[] = {1000, 1250, 1280, 1600, 2000, 2500, 3125,
                                   3200, 4000, 5000, 6250, 6400, 8000};
    static const int near[] = {9999, 10000, 10001, 100000};
    static const int telling[] = {1, 2, 17, 999, 1000, 1001, 1024, 2000,
                                  2048, 4096, 5000, 9999, 10000, 100000};
    static const int masses[] = {1, 99, 100, 101, 1000};
    int side[3], shape = (int)rnd(0, small ? 2 : 3), mass, i, k, t;
    long long base;

    if (shape == 0) {
        for (i = 0; i < 2; i++) {
            k = (int)rnd(0, 12);
            side[i] = rnd_chance(50) ? divisors[k] : (int)rnd(1000, 9999);
        }
        base = (long long)side[0] * side[1];
        side[2] = (int)((1000000000LL + base - 1) / base + rnd(-1, 1));
    } else if (shape == 1) {
        k = (int)rnd(0, 3);
        side[0] = rnd_chance(50) ? near[k] : (int)rnd(9990, 10010);
        side[1] = (int)rnd(1, 100);
        side[2] = (int)rnd(1, 100);
    } else if (shape == 2) {
        for (i = 0; i < 3; i++) {
            k = (int)rnd(0, 13);
            side[i] = telling[k];
        }
    } else {
        t = rnd_chance(50) ? 100000 : 9999;
        for (i = 0; i < 3; i++)
            side[i] = (int)rnd(1, t);
    }
    /* put the sides in a random order, so that each comes in each place */
    for (i = 2; i > 0; i--) {
        k = (int)rnd(0, i);
        t = side[i];
        side[i] = side[k];
        side[k] = t;
    }
    k = (int)rnd(0, 4);
    mass = rnd_chance(60) ? masses[k] : (int)rnd(1, 1000);
    for (i = 0; i < 3; i++)
        put_int(side[i]);
    put_int(mass);
}
