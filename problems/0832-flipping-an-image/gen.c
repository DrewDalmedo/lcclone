/* A slide of 1 to 5 dots a side for small cases; full-size ones are 20 a
 * side most of the time, else 19 or a random size up to 20. Each row is
 * random dots, dark with a chance the seed picks, or now and then a row
 * that reads the same both ways, or one in which each dot is the opposite
 * of its mirror dot, so that the two changes leave it as it is (all but
 * the middle dot of an odd row). Or the seed picks a slide that is all
 * clear, all dark, or a checkerboard. */
void gen(int small)
{
    int n, shape = (int)rnd(0, 9), pct = (int)rnd(10, 90), i, j, kind, v;
    int *a;

    if (small)
        n = (int)rnd(1, 5);
    else if (rnd_chance(60))
        n = 20;
    else if (rnd_chance(30))
        n = 19;
    else
        n = (int)rnd(1, 20);
    a = malloc((size_t)n * n * sizeof *a);
    for (i = 0; i < n; i++) {
        kind = (int)rnd(0, 9);
        for (j = 0; j < n; j++) {
            if (shape == 0) {
                v = 0;
            } else if (shape == 1) {
                v = 1;
            } else if (shape == 2) {
                v = (i + j) % 2;
            } else if (kind == 0 && j >= (n + 1) / 2) {
                /* reads the same both ways */
                v = a[i * n + n - 1 - j];
            } else if (kind == 1 && j >= (n + 1) / 2) {
                /* the opposite of its mirror dot: the change keeps it */
                v = 1 - a[i * n + n - 1 - j];
            } else {
                v = rnd_chance(pct);
            }
            a[i * n + j] = v;
        }
    }
    put_int_grid(a, n, n);
    free(a);
}
