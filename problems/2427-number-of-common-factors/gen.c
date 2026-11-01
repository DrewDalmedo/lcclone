/* The seed picks the shape: two random numbers; equal numbers; numbers
 * sharing a random factor g; one a larger multiple of the other; or numbers
 * with many divisors or perfect squares (840, 720, 960, 900, 961, ...). Small
 * cases stay at or below 60; full-size ones go up to 1000. */
void gen(int small)
{
    static const int rich_small[] = {1, 12, 16, 24, 36, 48, 49, 60};
    static const int rich_large[] = {1, 840, 720, 960, 900, 961, 997, 999,
                                     1000, 504, 630, 576};
    int top = small ? 60 : 1000, shape = (int)rnd(0, 4), a, b, g, u, t;

    if (shape == 0) {
        a = (int)rnd(1, top);
        b = (int)rnd(1, top);
    } else if (shape == 1) {
        a = (int)rnd(1, top);
        b = a;
    } else if (shape == 2) {
        g = (int)rnd(1, top / 2);
        a = g * (int)rnd(1, top / g);
        b = g * (int)rnd(1, top / g);
    } else if (shape == 3) {
        a = (int)rnd(1, top / 2);
        b = a * (int)rnd(2, top / a);
    } else {
        u = small ? (int)rnd(0, 7) : (int)rnd(0, 11);
        a = small ? rich_small[u] : rich_large[u];
        u = small ? (int)rnd(0, 7) : (int)rnd(0, 11);
        b = small ? rich_small[u] : rich_large[u];
    }
    if (rnd_chance(50)) {
        t = a;
        a = b;
        b = t;
    }
    put_int(a);
    put_int(b);
}
