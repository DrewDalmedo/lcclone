/* A list of tiles, each written either way round. Small cases have up to
 * 10 tiles whose numbers come from 1..3 (now and then 1..9), so equivalent
 * tiles, equal halves and turned copies come up often. Full-size cases
 * have 4 * 10^4 tiles, in a shape the seed picks:
 *   0: numbers from all of 1..9;
 *   1: every tile equivalent to one tile, written both ways round (the
 *      largest answer, 799980000);
 *   2: tiles drawn from just two or three kinds;
 *   3: 45 distinct kinds first (no pair among them), then tiles of the
 *      same kind as the last one, so pairs come only at the far end. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 10) : 40000;
    int shape = small ? -1 : (int)rnd(0, 3);
    int top = small ? (rnd_chance(20) ? 9 : 3) : 9;
    int *a = malloc((size_t)n * 2 * sizeof *a);
    int kinds[3][2], k, nk, i, x, y, t;

    x = (int)rnd(1, 9);
    y = (int)rnd(1, 9);
    nk = (int)rnd(2, 3);
    for (k = 0; k < nk; k++) {
        kinds[k][0] = (int)rnd(1, 9);
        kinds[k][1] = (int)rnd(1, 9);
    }
    k = 0;
    for (i = 0; i < n; i++) {
        if (shape == 1) {
            a[2 * i] = x;
            a[2 * i + 1] = y;
        } else if (shape == 2) {
            t = (int)rnd(0, nk - 1);
            a[2 * i] = kinds[t][0];
            a[2 * i + 1] = kinds[t][1];
        } else if (shape == 3 && k < 45) {
            /* the k-th kind (x, y) with 1 <= x <= y <= 9, in order */
            for (x = 1, t = k; t >= 10 - x; x++)
                t -= 10 - x;
            a[2 * i] = x;
            a[2 * i + 1] = x + t;
            k++;
        } else if (shape == 3) {
            a[2 * i] = 9;
            a[2 * i + 1] = 9;
        } else {
            a[2 * i] = (int)rnd(1, top);
            a[2 * i + 1] = (int)rnd(1, top);
        }
        if (rnd_chance(50)) {
            t = a[2 * i];
            a[2 * i] = a[2 * i + 1];
            a[2 * i + 1] = t;
        }
    }
    put_int_grid(a, n, 2);
    free(a);
}
