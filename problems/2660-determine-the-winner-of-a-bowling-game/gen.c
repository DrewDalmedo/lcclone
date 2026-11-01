/* Two players' rounds, with strikes common. The seed picks how player 2's
 * rounds relate to player 1's: drawn on their own, the same pins in
 * another order (equal pin totals, so only the doubling decides), or a
 * copy with one round changed (a close game, or a draw). Small cases have
 * up to 8 rounds; full-size ones have 1000, and some are nearly all
 * strikes. */
static int pins(int strike_percent)
{
    if (rnd_chance(strike_percent))
        return 10;
    return (int)rnd(0, 10);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000;
    int strikes = small ? 30 : (rnd_chance(30) ? 95 : (int)rnd(10, 60));
    int shape = (int)rnd(0, 2);
    int *a = malloc((size_t)n * sizeof *a);
    int *b = malloc((size_t)n * sizeof *b);
    int i, at;

    for (i = 0; i < n; i++)
        a[i] = pins(strikes);
    if (shape == 0) {
        for (i = 0; i < n; i++)
            b[i] = pins(strikes);
    } else {
        for (i = 0; i < n; i++)
            b[i] = a[i];
        if (shape == 1) {
            rnd_shuffle(b, n);
        } else {
            at = (int)rnd(0, n - 1);
            b[at] = pins(strikes);
        }
    }
    if (rnd_chance(50)) {
        put_ints(a, n);
        put_ints(b, n);
    } else {
        put_ints(b, n);
        put_ints(a, n);
    }
    free(a);
    free(b);
}
