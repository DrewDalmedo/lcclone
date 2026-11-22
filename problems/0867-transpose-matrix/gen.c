/* A matrix. Small cases are up to 4 x 4, with numbers from -9 to 9.
 * Full-size ones have 10^5 numbers, at most 1000 in a row or a column, in
 * a shape the seed picks: 100 rows of 1000, 1000 rows of 100, or 250 rows
 * of 400; their numbers are -10^9 and 10^9 at random, or anything between. */
void gen(int small)
{
    static const int shapes[3][2] = {{100, 1000}, {1000, 100}, {250, 400}};
    int m, n, i, ends = rnd_chance(50);
    int *a;

    if (small) {
        m = (int)rnd(1, 4);
        n = (int)rnd(1, 4);
    } else {
        m = shapes[gen_seed() % 3][0];
        n = shapes[gen_seed() % 3][1];
    }
    a = malloc((size_t)m * (size_t)n * sizeof *a);
    for (i = 0; i < m * n; i++)
        if (small)
            a[i] = (int)rnd(-9, 9);
        else if (ends)
            a[i] = rnd_chance(50) ? -1000000000 : 1000000000;
        else
            a[i] = (int)rnd(-1000000000, 1000000000);
    put_int_grid(a, m, n);
    free(a);
}
