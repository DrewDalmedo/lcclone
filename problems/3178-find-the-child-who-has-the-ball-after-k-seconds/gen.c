/* Small cases: a short line (often just 2 or 3 children, so the ball makes
 * many legs) and any k. Full-size ones reach the bounds: n and k up to 50,
 * with the seed picking n = 50, k = 50, or k at a turning point (one leg,
 * a round trip, or one second past either). */
void gen(int small)
{
    int n, k, legs, shape;

    if (small) {
        n = rnd_chance(30) ? (int)rnd(2, 3) : (int)rnd(2, 10);
        k = (int)rnd(1, 50);
    } else {
        shape = (int)rnd(0, 3);
        n = shape == 1 ? 50 : (int)rnd(2, 50);
        k = shape == 2 ? 50 : (int)rnd(1, 50);
        if (shape == 3) {
            /* k at the end of a leg, or one second after it */
            n = (int)rnd(2, 26);
            legs = (int)rnd(1, 49 / (n - 1));
            k = legs * (n - 1) + (int)rnd(0, 1);
        }
    }
    put_int(n);
    put_int(k);
}
