/* Strictly increasing upper bounds, chosen in order from 1..top (each
 * value kept with the chance that leaves exactly m by the end), random
 * rates with 0% and 100% often, and an income up to the last bound,
 * often exactly on a bound or 0. */
void gen(int small)
{
    int m = small ? (int)rnd(1, 5) : 100, top = small ? 20 : 1000;
    int *b = malloc((size_t)m * 2 * sizeof *b);
    int v, i = 0, income;

    for (v = 1; v <= top && i < m; v++)
        if (rnd(0, top - v) < m - i)
            b[2 * i++] = v;
    for (i = 0; i < m; i++) {
        int r = (int)rnd(0, 9);

        b[2 * i + 1] = r == 0 ? 0 : r == 1 ? 100 : (int)rnd(0, 100);
    }
    if (rnd_chance(10))
        income = 0;
    else if (rnd_chance(30))
        income = b[2 * rnd(0, m - 1)];
    else
        income = (int)rnd(0, b[2 * (m - 1)]);
    put_int_grid(b, m, 2);
    put_int(income);
    free(b);
}
