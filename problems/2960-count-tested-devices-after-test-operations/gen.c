/* Charges for the row of gadgets. Small cases have 1 to 8 gadgets;
 * full-size ones 100 most of the time, else a random number up to 100.
 * The seed picks the shape:
 *   0-1: random charges, 0 a third of the time, else up to a small top
 *        (1 to 5) in small cases or up to 100 in full-size ones;
 *   2:   each charge within 1 of the number tested so far, so gadgets
 *        whose charge equals that number (and are skipped) come up often;
 *   3:   charges rising by 0 to 2 from gadget to gadget, from 0 or 1;
 *   4:   charges falling from a top (100 in full-size cases) by 0 to 2;
 *   5:   every charge the same, 100 one time in two in full-size cases. */
void gen(int small)
{
    int n, shape, top, tested = 0, i, v, step;
    int *b;

    if (small)
        n = (int)rnd(1, 8);
    else if (rnd_chance(70))
        n = 100;
    else
        n = (int)rnd(1, 100);
    b = malloc((size_t)n * sizeof *b);
    shape = (int)rnd(0, 5);
    top = small ? (int)rnd(1, 5) : 100;
    v = 0;
    if (shape == 3)
        v = (int)rnd(0, 1);
    else if (shape == 4)
        v = small ? (int)rnd(1, 9) : 100;
    else if (shape == 5)
        v = !small && rnd_chance(50) ? 100 : (int)rnd(0, small ? 6 : 100);
    for (i = 0; i < n; i++) {
        if (shape <= 1) {
            if (rnd_chance(33))
                b[i] = 0;
            else
                b[i] = (int)rnd(1, top);
        } else if (shape == 2) {
            step = (int)rnd(-1, 1);
            b[i] = tested + step < 0 ? 0 : tested + step;
        } else if (shape == 3 || shape == 4) {
            step = (int)rnd(0, 2);
            b[i] = v;
            v = shape == 3 ? v + step : v - step;
            if (v > 100)
                v = 100;
            if (v < 0)
                v = 0;
        } else {
            b[i] = v;
        }
        if (b[i] > tested)
            tested++;
    }
    put_ints(b, n);
    free(b);
}
