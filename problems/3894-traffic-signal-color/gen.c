/* One reading. Small cases stay close to the borders of the states: the
 * seed picks one of 0, 30, 90 and 1000 and moves up to 3 away from it,
 * staying in 0..1000, or now and then takes any reading up to 120.
 * Full-size cases take any reading from 0 to 1000. */
void gen(int small)
{
    static const int border[] = {0, 30, 90, 1000};
    int t, b, d;

    if (!small) {
        put_int(rnd(0, 1000));
        return;
    }
    if (rnd_chance(25)) {
        put_int(rnd(0, 120));
        return;
    }
    b = border[rnd(0, 3)];
    d = (int)rnd(-3, 3);
    t = b + d;
    if (t < 0)
        t = -t;
    if (t > 1000)
        t = 2000 - t;
    put_int(t);
}
