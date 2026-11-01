/* Rectangles as [l, w] pairs, each side in random order. Two diagonals tie
 * when l^2 + w^2 is a sum of two squares in more than one way (50 = 1 + 49
 * = 25 + 25), so each case draws rows from such a family of pairs, which
 * gives ties on the diagonal with different areas.
 * Small cases: up to 8 rows; the family is one of the nine smallest (sums
 * 50 to 200, sides up to 14), and the other rows have sides up to one more
 * than the family's longest side, so repeats, swaps and ties are common.
 * Full-size cases: 100 rows, in a shape the seed picks:
 *   0: sides from all of 1..100;
 *   1: a family of 4 to 6 pairs with a sum from 5525 to 11050 holds the
 *      longest diagonal, in random order; every other row is shorter;
 *   2: sides up to 99, then [100,100] last: the answer is at the end;
 *   3: [1,100] first, then shorter diagonals with larger areas. */
static int family(int sum, int *fa, int *fb)
{
    int n = 0, x, y;

    for (x = 1; x <= 100; x++)
        for (y = x; y <= 100; y++)
            if (x * x + y * y == sum) {
                fa[n] = x;
                fb[n] = y;
                n++;
            }
    return n;
}

void gen(int small)
{
    static const int small_sums[] = {50, 65, 85, 125, 130, 145, 170, 185, 200};
    static const int big_sums[] = {5525, 8125, 8450, 9425, 10625, 11050};
    int shape = small ? -1 : (int)(gen_seed() % 4);
    int n = small ? (int)rnd(1, 8) : 100;
    int sum = small ? small_sums[rnd(0, 8)] : big_sums[rnd(0, 5)];
    int fa[8], fb[8], f = family(sum, fa, fb), top = fb[0];
    int rows[200], order[100], out[200], i, k, x, y;

    for (i = 0; i < n; i++) {
        if (shape == -1) {
            if (rnd_chance(45)) {
                k = (int)rnd(0, f - 1);
                x = fa[k];
                y = fb[k];
            } else {
                x = (int)rnd(1, top + 1);
                y = (int)rnd(1, top + 1);
            }
        } else if (shape == 0) {
            x = (int)rnd(1, 100);
            y = (int)rnd(1, 100);
        } else if (shape == 1 && (i < f || rnd_chance(10))) {
            k = i < f ? i : (int)rnd(0, f - 1);
            x = fa[k];
            y = fb[k];
        } else if (shape == 2) {
            x = i == n - 1 ? 100 : (int)rnd(1, 99);
            y = i == n - 1 ? 100 : (int)rnd(1, 99);
        } else if (shape == 3 && i == 0) {
            x = 1;
            y = 100;
        } else {
            /* shape 1 or 3: shorter than the family, or than [1,100] */
            do {
                x = (int)rnd(1, 100);
                y = (int)rnd(1, 100);
            } while (x * x + y * y >= (shape == 1 ? sum : 10001));
        }
        if (rnd_chance(50)) {
            rows[2 * i] = x;
            rows[2 * i + 1] = y;
        } else {
            rows[2 * i] = y;
            rows[2 * i + 1] = x;
        }
    }
    for (i = 0; i < n; i++)
        order[i] = i;
    if (shape == 1)
        rnd_shuffle(order, n);
    for (i = 0; i < n; i++) {
        out[2 * i] = rows[2 * order[i]];
        out[2 * i + 1] = rows[2 * order[i] + 1];
    }
    put_int_grid(out, n, 2);
}
