/* Small cases: boards of 1 to 4 cells a side with heights from 0 to 4,
 * about a third of the cells empty, so that equal neighbours, empty cells
 * shut in by stacks and lone cubes come up often. Full-size boards are 50
 * by 50 most of the time, else of a random size, and the seed picks the
 * heights:
 *   - random from 0 to 50;
 *   - a checkerboard of stacks and empty cells, the stacks 50 high or
 *     random (no two stacks share a side: the largest areas);
 *   - every cell 50, or every cell one random height;
 *   - random, with most cells empty;
 *   - stripes of 50 and 1, so that every two neighbours across the stripes
 *     differ. */
void gen(int small)
{
    int n, shape = (int)rnd(0, 5), parity = (int)rnd(0, 1), i, j, v;
    int same = (int)rnd(1, 50);
    int *g;

    if (small)
        n = (int)rnd(1, 4);
    else if (rnd_chance(70))
        n = 50;
    else
        n = (int)rnd(1, 50);
    g = malloc((size_t)n * n * sizeof *g);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            if (small) {
                v = rnd_chance(35) ? 0 : (int)rnd(1, 4);
            } else if (shape <= 1) {
                v = (int)rnd(0, 50);
            } else if (shape == 2) {
                if ((i + j) % 2 != parity)
                    v = 0;
                else if (rnd_chance(60))
                    v = 50;
                else
                    v = (int)rnd(1, 50);
            } else if (shape == 3) {
                v = parity ? 50 : same;
            } else if (shape == 4) {
                v = rnd_chance(85) ? 0 : (int)rnd(1, 50);
            } else {
                v = (parity ? i : j) % 2 ? 50 : 1;
            }
            g[i * n + j] = v;
        }
    put_int_grid(g, n, n);
    free(g);
}
