/* Only n = 1, 8, 49 and 288 have a balance rung, so a third of the cases
 * take one of them or a neighbour; the rest are small n, or for full-size
 * cases any n, often close to the top bound, 1000. */
void gen(int small)
{
    static const int pivots[] = {1, 8, 49, 288};
    int n, k, step;

    if (rnd_chance(35)) {
        k = (int)rnd(0, 3);
        step = (int)rnd(-1, 1);
        n = pivots[k] + step;
        if (n < 1)
            n = 1;
    } else if (small) {
        n = (int)rnd(1, 60);
    } else {
        n = rnd_chance(50) ? (int)rnd(900, 1000) : (int)rnd(1, 1000);
    }
    put_int(n);
}
