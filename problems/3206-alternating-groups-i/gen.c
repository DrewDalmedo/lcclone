/* Random loops. The seed picks a shape: colors drawn at random; a fully
 * alternating loop with a few beads flipped (an odd n leaves a seam where
 * two equal beads meet); or a single color with a few beads flipped.
 * Small cases have 3 to 8 beads, full-size ones 95 to 100. */
void gen(int small)
{
    int n = small ? (int)rnd(3, 8) : (int)rnd(95, 100), i;
    int shape = (int)rnd(0, 2);
    int first = (int)rnd(0, 1);
    int flips = (int)rnd(0, small ? 2 : 4), at;
    int *colors = malloc((size_t)n * sizeof *colors);

    for (i = 0; i < n; i++) {
        if (shape == 0)
            colors[i] = (int)rnd(0, 1);
        else if (shape == 1)
            colors[i] = (first + i) % 2;
        else
            colors[i] = first;
    }
    if (shape != 0)
        for (i = 0; i < flips; i++) {
            at = (int)rnd(0, n - 1);
            colors[at] ^= 1;
        }
    put_ints(colors, n);
    free(colors);
}
