/* A striped grid (each column one value, neighbouring columns different),
 * then the seed picks what to spoil: nothing, one random cell, a column
 * made equal to its right neighbour, or every cell drawn at random. Small
 * cases are up to 4 x 4 with values up to 1..9; full-size ones are 10 x 10
 * or close, where a spoiled cell is often the bottom-right corner. */
void gen(int small)
{
    int g[10][10], col[10];
    int m, n, top, i, j, v, shape;

    if (small) {
        m = (int)rnd(1, 4);
        n = (int)rnd(1, 4);
        top = (int)rnd(1, 9);
    } else {
        m = rnd_chance(50) ? 10 : (int)rnd(7, 10);
        n = rnd_chance(50) ? 10 : (int)rnd(7, 10);
        top = 9;
    }
    for (j = 0; j < n; j++) {
        do
            v = (int)rnd(0, top);
        while (j > 0 && v == col[j - 1]);
        col[j] = v;
    }
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            g[i][j] = col[j];
    shape = (int)rnd(0, 3);
    if (shape == 1) {
        if (!small && rnd_chance(50)) {
            i = m - 1;
            j = n - 1;
        } else {
            i = (int)rnd(0, m - 1);
            j = (int)rnd(0, n - 1);
        }
        g[i][j] = (int)rnd(0, top);
    } else if (shape == 2 && n >= 2) {
        j = (int)rnd(0, n - 2);
        for (i = 0; i < m; i++)
            g[i][j] = g[i][j + 1];
    } else if (shape == 3) {
        for (i = 0; i < m; i++)
            for (j = 0; j < n; j++)
                g[i][j] = (int)rnd(0, small ? 2 : top);
    }
    {
        int flat[100];

        for (i = 0; i < m; i++)
            for (j = 0; j < n; j++)
                flat[i * n + j] = g[i][j];
        put_int_grid(flat, m, n);
    }
}
