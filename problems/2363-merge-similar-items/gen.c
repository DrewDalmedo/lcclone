/* Two lists of [label, weight] rows, labels distinct within each list.
 * Small cases: up to 6 rows each, labels from a pool barely larger than
 * the longer list (at the low end, at the high end up to 1000, or
 * scattered), so that labels are often shared; weights mostly 1..9,
 * sometimes near 1000. Full-size cases, by the seed: both lists carry
 * every label 1..1000, the labels split 500/500 with none shared, 1000
 * labels against a random subset, or random lengths. Rows come shuffled. */
static int pool[1000];

/* Prints n rows labelled labels[0..n), each with a random weight. */
static void put_rows(const int *labels, int n, int small)
{
    static int rows[2000];
    int i, w;

    for (i = 0; i < n; i++) {
        if (!small)
            w = (int)rnd(1, 1000);
        else if (rnd_chance(15))
            w = (int)rnd(990, 1000);
        else
            w = (int)rnd(1, 9);
        rows[2 * i] = labels[i];
        rows[2 * i + 1] = w;
    }
    put_int_grid(rows, n, 2);
}

void gen(int small)
{
    int n1, n2, longer, size, mode, shape, v, dup, i, j;

    if (small) {
        n1 = (int)rnd(1, 6);
        n2 = (int)rnd(1, 6);
        longer = n1 > n2 ? n1 : n2;
        size = (int)rnd(longer, longer + 4);
        mode = (int)rnd(0, 2);
        for (i = 0; i < size; i++) {
            if (mode == 0) {
                pool[i] = i + 1;
            } else if (mode == 1) {
                pool[i] = 1000 - i;
            } else {
                do {
                    v = (int)rnd(1, 1000);
                    for (dup = 0, j = 0; j < i; j++)
                        if (pool[j] == v)
                            dup = 1;
                } while (dup);
                pool[i] = v;
            }
        }
    } else {
        size = 1000;
        for (i = 0; i < size; i++)
            pool[i] = i + 1;
        shape = (int)rnd(0, 3);
        if (shape == 0) {
            n1 = n2 = 1000;
        } else if (shape == 1) {
            rnd_shuffle(pool, size);
            put_rows(pool, 500, 0);
            put_rows(pool + 500, 500, 0);
            return;
        } else if (shape == 2) {
            n1 = 1000;
            n2 = (int)rnd(1, 1000);
        } else {
            n1 = (int)rnd(1, 1000);
            n2 = (int)rnd(1, 1000);
        }
    }
    /* Each list takes the first labels of a fresh shuffle of the pool. */
    rnd_shuffle(pool, size);
    put_rows(pool, n1, small);
    rnd_shuffle(pool, size);
    put_rows(pool, n2, small);
}
