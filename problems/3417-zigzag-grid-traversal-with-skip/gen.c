/* A random grid. Small: 2 to 6 rows and columns with weights 1 to 9, so
 * that equal weights are common (sometimes all of them). Full size: the
 * seed picks the shape (50 x 50, 49 x 49, 49 x 50, 50 x 49, 2 x 50 or
 * 50 x 2), with weights over the whole range. */
void gen(int small)
{
    static const int shapes[6][2] = {
        {50, 50}, {49, 49}, {49, 50}, {50, 49}, {2, 50}, {50, 2}
    };
    int rows, cols, top, i, same;
    int *g;

    if (small) {
        rows = (int)rnd(2, 6);
        cols = (int)rnd(2, 6);
        top = rnd_chance(25) ? 2500 : 9;
    } else {
        rows = shapes[gen_seed() % 6][0];
        cols = shapes[gen_seed() % 6][1];
        top = 2500;
    }
    g = malloc((size_t)rows * cols * sizeof *g);
    same = small && rnd_chance(10);
    for (i = 0; i < rows * cols; i++)
        g[i] = same && i > 0 ? g[0] : (int)rnd(1, top);
    put_int_grid(g, rows, cols);
    free(g);
}
