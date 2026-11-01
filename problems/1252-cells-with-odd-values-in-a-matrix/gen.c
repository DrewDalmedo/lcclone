/* A board of 1 to 5 by 1 to 5 squares with 1 to 8 steps for small cases
 * (now and then 20 to 100 steps); full-size ones have 100 steps on a board
 * whose sides are 50 most of the time, else random. The seed picks the
 * steps:
 *   0-5: random squares (small cases often repeat them);
 *   6:   every step on one square, or all but one, which goes to another
 *        column of the same row (a square reaches 200 or 199, past what a
 *        signed char holds);
 *   7:   steps drawn from just two squares, so most of them cancel;
 *   8:   one step on each row, all in one column (m steps): with m even,
 *        every square ends up odd;
 *   9:   random rows, with the columns repeated in pairs, so the columns
 *        end up even (but for the last one when the count is odd). */
void gen(int small)
{
    int m, n, k, shape = (int)rnd(0, 9), r0, c0, r1, c1, i, r, c;
    int *ops;

    if (small) {
        m = (int)rnd(1, 5);
        n = (int)rnd(1, 5);
        k = rnd_chance(10) ? (int)rnd(20, 100) : (int)rnd(1, 8);
    } else {
        m = rnd_chance(60) ? 50 : (int)rnd(1, 50);
        n = rnd_chance(60) ? 50 : (int)rnd(1, 50);
        k = 100;
    }
    if (shape == 8)
        k = m;
    ops = malloc((size_t)2 * k * sizeof *ops);
    r0 = (int)rnd(0, m - 1);
    c0 = (int)rnd(0, n - 1);
    r1 = (int)rnd(0, m - 1);
    c1 = (int)rnd(0, n - 1);
    for (i = 0; i < k; i++) {
        if (shape == 6) {
            r = r0;
            c = c0;
        } else if (shape == 7) {
            if (rnd_chance(50)) {
                r = r0;
                c = c0;
            } else {
                r = r1;
                c = c1;
            }
        } else if (shape == 8) {
            r = i;
            c = c0;
        } else if (shape == 9) {
            r = (int)rnd(0, m - 1);
            c = i % 2 ? ops[2 * i - 1] : (int)rnd(0, n - 1);
        } else {
            r = (int)rnd(0, m - 1);
            c = (int)rnd(0, n - 1);
        }
        ops[2 * i] = r;
        ops[2 * i + 1] = c;
    }
    if (shape == 6 && rnd_chance(50)) {
        i = (int)rnd(0, k - 1);
        ops[2 * i + 1] = c1;
    }
    put_int(m);
    put_int(n);
    put_int_grid(ops, k, 2);
    free(ops);
}
