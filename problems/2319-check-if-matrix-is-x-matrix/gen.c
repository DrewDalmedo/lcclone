/* A square grid that draws an X, then maybe spoiled. The seed picks: no
 * change (an X-matrix); a marked cell set to 0; an unmarked cell made
 * non-zero; both at once, so the count of non-zero cells is still right;
 * the anti-diagonal drawn one place off (i + j == n); or random cells.
 * Values are 1 to 9 or up to 10^5, and sometimes all 65536, whose product
 * wraps to 0. Small cases have sides 3 to 8; full-size ones 99 or 100,
 * where a spoiled cell is often in the last row. */
static int is_marked(int n, int i, int j)
{
    return i == j || i + j == n - 1;
}

static int value(int top, int all_big)
{
    return all_big ? 65536 : (int)rnd(1, top);
}

void gen(int small)
{
    static const int kinds[] = {0, 0, 0, 1, 2, 3, 4, 5};
    int n = small ? (int)rnd(3, 8) : (int)rnd(99, 100);
    int top = small && rnd_chance(50) ? 9 : 100000;
    int all_big = rnd_chance(15);
    int kind = kinds[rnd(0, 7)];
    int last_row = !small && rnd_chance(50);
    int *a = malloc((size_t)n * n * sizeof *a);
    int i, j, v;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            if (kind == 4)
                a[i * n + j] = rnd_chance(40) ? value(top, all_big) : 0;
            else if (kind == 5)
                a[i * n + j] = i == j || i + j == n ? value(top, all_big) : 0;
            else
                a[i * n + j] = is_marked(n, i, j) ? value(top, all_big) : 0;
        }
    if (kind == 1 || kind == 3) {
        i = last_row ? n - 1 : (int)rnd(0, n - 1);
        j = rnd_chance(50) ? i : n - 1 - i;
        a[i * n + j] = 0;
    }
    if (kind == 2 || kind == 3) {
        do {
            i = last_row ? n - 1 : (int)rnd(0, n - 1);
            j = (int)rnd(0, n - 1);
        } while (is_marked(n, i, j));
        v = value(top, all_big);
        a[i * n + j] = v;
    }
    put_int_grid(a, n, n);
    free(a);
}
