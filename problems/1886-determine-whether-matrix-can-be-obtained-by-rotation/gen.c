/* mat is a random 0/1 grid: n from 1 to 4 for small cases, 10 (or 9, for
 * an odd size with a centre cell) for full-size ones. Its cells are 1 with
 * one chance from 20% to 80%, or (one time in ten) all 0 or all 1. The seed
 * then picks target:
 *   - mat itself (twice as likely as the other shapes);
 *   - mat with one cell changed: the last cell, the centre, or any;
 *   - a mirror image of mat: its transpose, flipped left to right, flipped
 *     upside down, or flipped across the other diagonal;
 *   - a random grid with the same chance of 1s;
 * and finally turns target clockwise 0 to 3 quarter turns. */
static void turn(int *g, int n)
{
    int t[100], i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            t[j * n + (n - 1 - i)] = g[i * n + j];
    memcpy(g, t, (size_t)(n * n) * sizeof *g);
}

void gen(int small)
{
    int mat[100], target[100], n, kind, pct, shape, mirror, where, cell;
    int turns, i, j;

    n = small ? (int)rnd(1, 4) : (rnd_chance(75) ? 10 : 9);
    kind = (int)rnd(0, 19);
    pct = kind == 0 ? 0 : kind == 1 ? 100 : (int)rnd(20, 80);
    for (i = 0; i < n * n; i++)
        mat[i] = rnd_chance(pct);
    memcpy(target, mat, sizeof mat);
    shape = (int)rnd(0, 4);
    if (shape == 2) {
        where = (int)rnd(0, 3);
        if (where == 0)
            cell = n * n - 1;
        else if (where == 1)
            cell = (n / 2) * n + n / 2;
        else
            cell = (int)rnd(0, n * n - 1);
        target[cell] ^= 1;
    } else if (shape == 3) {
        mirror = (int)rnd(0, 3);
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++) {
                if (mirror == 0)
                    target[i * n + j] = mat[j * n + i];
                else if (mirror == 1)
                    target[i * n + j] = mat[i * n + (n - 1 - j)];
                else if (mirror == 2)
                    target[i * n + j] = mat[(n - 1 - i) * n + j];
                else
                    target[i * n + j] = mat[(n - 1 - j) * n + (n - 1 - i)];
            }
    } else if (shape == 4) {
        for (i = 0; i < n * n; i++)
            target[i] = rnd_chance(pct);
    }
    turns = (int)rnd(0, 3);
    for (i = 0; i < turns; i++)
        turn(target, n);
    put_int_grid(mat, n, n);
    put_int_grid(target, n, n);
}
