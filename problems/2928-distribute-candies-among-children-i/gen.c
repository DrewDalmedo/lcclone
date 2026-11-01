/* Small cases keep both numbers low, so that n often lands near 3 * limit
 * (one way, three ways, none) or below limit (the cap never matters).
 * Full-size ones range over the whole constraints; the seed picks the
 * shape: anything, a cap that never binds, or n right around 3 * limit. */
void gen(int small)
{
    int n, limit, shape = (int)rnd(0, 3), d;

    if (small) {
        limit = (int)rnd(1, 6);
        n = (int)rnd(1, 3 * limit + 2);
    } else if (shape == 0) {
        limit = (int)rnd(1, 50);
        n = (int)rnd(1, 50);
    } else if (shape == 1) {
        n = (int)rnd(1, 50);
        limit = (int)rnd(n, 50);
    } else {
        limit = (int)rnd(1, 17);
        d = (int)rnd(-3, 2);
        n = 3 * limit + d;
        if (n < 1)
            n = 1;
        if (n > 50)
            n = 50;
    }
    put_int(n);
    put_int(limit);
}
