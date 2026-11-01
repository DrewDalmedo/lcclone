/* A board holding 1 to n * n with one number left off and another on two
 * squares. Small cases have n from 2 to 4; full-size ones n = 50 half the
 * time, else n from 32 to 50 (where sums of squares, or the product
 * N * (N + 1) * (2N + 1), no longer fit an int) or, one time in five,
 * from 2 to 50.
 * The missing number is 1 or n * n two times in five, else any; the
 * number on two squares is just below or just above it, 1 or n * n, or
 * any other number. The seed picks where its two copies go: on random
 * squares (half the time), on the first and the last square, in one
 * column, side by side in one row, or with the whole board in increasing
 * order, row by row. */
void gen(int small)
{
    int n, total, missing, twice, place, i, k, v, p1, p2, t;
    int *rest, *cells;

    if (small)
        n = (int)rnd(2, 4);
    else if (rnd_chance(50))
        n = 50;
    else if (rnd_chance(80))
        n = (int)rnd(32, 50);
    else
        n = (int)rnd(2, 50);
    total = n * n;
    switch (rnd(0, 4)) {
    case 0:
        missing = 1;
        break;
    case 1:
        missing = total;
        break;
    default:
        missing = (int)rnd(1, total);
    }
    switch (rnd(0, 4)) {
    case 0:
        twice = missing - 1;
        break;
    case 1:
        twice = missing + 1;
        break;
    case 2:
        twice = rnd_chance(50) ? 1 : total;
        break;
    default:
        twice = missing;
    }
    while (twice < 1 || twice > total || twice == missing)
        twice = (int)rnd(1, total);
    rest = malloc((size_t)total * sizeof *rest);
    cells = malloc((size_t)total * sizeof *cells);
    k = 0;
    for (v = 1; v <= total; v++)
        if (v != missing && v != twice)
            rest[k++] = v;
    place = (int)rnd(0, 7);
    if (place == 7) {
        k = 0;
        for (v = 1; v <= total; v++) {
            if (v != missing)
                cells[k++] = v;
            if (v == twice)
                cells[k++] = v;
        }
    } else {
        rnd_shuffle(rest, total - 2);
        if (place == 4) {
            p1 = 0;
            p2 = total - 1;
        } else if (place == 5) {
            p1 = (int)rnd(0, total - n - 1);
            t = (int)rnd(1, (total - 1 - p1) / n);
            p2 = p1 + t * n;
        } else if (place == 6) {
            t = (int)rnd(0, n - 1);
            p1 = t * n + (int)rnd(0, n - 2);
            p2 = p1 + 1;
        } else {
            p1 = (int)rnd(0, total - 1);
            do
                p2 = (int)rnd(0, total - 1);
            while (p2 == p1);
        }
        k = 0;
        for (i = 0; i < total; i++)
            cells[i] = i == p1 || i == p2 ? twice : rest[k++];
    }
    put_int_grid(cells, n, n);
    free(rest);
    free(cells);
}
