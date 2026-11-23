/* A picture of 1 to 5 by 1 to 5 pixels for small cases; full-size ones are
 * 200 by 200. The brightnesses are drawn for small cases and picked by the
 * seed for full-size ones:
 *   - any value from 0 to 255;
 *   - only 0 to 3, so that sums are small and many averages tie;
 *   - one value everywhere;
 *   - only 250 to 255, so that sums are near their largest;
 *   - only 0 and 255, so that averages often end in a half. */
void gen(int small)
{
    int m, n, k, v, kind = (int)rnd(0, 4);
    int *img;

    if (small) {
        m = (int)rnd(1, 5);
        n = (int)rnd(1, 5);
    } else {
        m = 200;
        n = 200;
        kind = (int)(gen_seed() % 5);
    }
    img = malloc((size_t)m * n * sizeof *img);
    v = (int)rnd(0, 255);
    for (k = 0; k < m * n; k++) {
        if (kind == 0)
            img[k] = (int)rnd(0, 255);
        else if (kind == 1)
            img[k] = (int)rnd(0, 3);
        else if (kind == 2)
            img[k] = v;
        else if (kind == 3)
            img[k] = (int)rnd(250, 255);
        else
            img[k] = rnd_chance(50) ? 255 : 0;
    }
    put_int_grid(img, m, n);
    free(img);
}
