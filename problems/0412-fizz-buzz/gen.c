/* Small cases count to at most 50, and half the time to a number next to
 * a multiple of 3, 5 or 15, so that the round ends on each kind of call.
 * Full-size ones count to 10^4 (even seeds) or to a random n up to 10^4. */
void gen(int small)
{
    static const int near[] = {3, 5, 15, 30, 45};
    long long n;
    int base, step;

    if (small) {
        if (rnd_chance(50)) {
            base = near[rnd(0, 4)];
            step = (int)rnd(-1, 1);
            n = base + step;
        } else {
            n = rnd(1, 50);
        }
    } else {
        n = gen_seed() % 2 == 0 ? 10000 : rnd(1, 10000);
    }
    put_int(n);
}
