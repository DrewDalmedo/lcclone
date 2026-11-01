/* Small cases: up to 8 islands and a random set of distinct footbridges
 * (often too few to link everything), each written either way round, in
 * a random order; destination is sometimes source itself. Full-size cases
 * have 2 * 10^5 islands, in one of four shapes picked by the seed:
 *   0  one long path through every island in a random order, plus one
 *      chord (2 * 10^5 bridges), listed from the far end back, with
 *      source and destination at the two ends;
 *   1  a star, its centre written first in every bridge;
 *   2  the same, its centre written second (each of these makes long
 *      chains for a union-find that has neither path compression nor
 *      union by size);
 *   3  two random trees, source in one and destination in the other. */

/* Appends the bridge u-v to e, written in a random direction. */
static void bridge(int *e, int *m, int u, int v)
{
    if (rnd_chance(50)) {
        e[2 * *m] = u;
        e[2 * *m + 1] = v;
    } else {
        e[2 * *m] = v;
        e[2 * *m + 1] = u;
    }
    (*m)++;
}

/* Shuffles the m bridges of e as whole pairs. */
static void shuffle_bridges(int *e, int m)
{
    int i, k, t;

    for (i = m - 1; i > 0; i--) {
        k = (int)rnd(0, i);
        t = e[2 * i];
        e[2 * i] = e[2 * k];
        e[2 * k] = t;
        t = e[2 * i + 1];
        e[2 * i + 1] = e[2 * k + 1];
        e[2 * k + 1] = t;
    }
}

static void small_case(void)
{
    int n = (int)rnd(1, 8), pairs[28], e[56], np = 0, m = 0, want, u, v, i;
    int source, destination;

    for (u = 0; u < n; u++)
        for (v = u + 1; v < n; v++)
            pairs[np++] = u * n + v;
    rnd_shuffle(pairs, np);
    want = (int)rnd(0, np < n ? np : n);
    for (i = 0; i < want; i++)
        bridge(e, &m, pairs[i] / n, pairs[i] % n);
    source = (int)rnd(0, n - 1);
    destination = rnd_chance(15) ? source : (int)rnd(0, n - 1);
    put_int(n);
    put_int_grid(e, m, 2);
    put_int(source);
    put_int(destination);
}

void gen(int small)
{
    int n = 200000, shape, m = 0, centre, half, i, k;
    int *order, *e;
    int source, destination;

    if (small) {
        small_case();
        return;
    }
    shape = (int)rnd(0, 3);
    order = malloc((size_t)n * sizeof *order);
    e = malloc((size_t)2 * n * sizeof *e);
    for (i = 0; i < n; i++)
        order[i] = i;
    rnd_shuffle(order, n);
    if (shape == 0) {
        for (i = n - 1; i > 0; i--)
            bridge(e, &m, order[i - 1], order[i]);
        bridge(e, &m, order[0], order[2]);
        source = order[0];
        destination = order[n - 1];
    } else if (shape == 1 || shape == 2) {
        centre = order[0];
        for (i = 0; i < n; i++) {
            if (i == centre)
                continue;
            e[2 * m] = shape == 1 ? centre : i;
            e[2 * m + 1] = shape == 1 ? i : centre;
            m++;
        }
        source = order[1];
        destination = order[n - 1];
    } else {
        half = (int)rnd(1, n - 1);
        for (i = 1; i < n; i++) {
            if (i == half)
                continue;
            k = i < half ? (int)rnd(0, i - 1) : (int)rnd(half, i - 1);
            bridge(e, &m, order[k], order[i]);
        }
        shuffle_bridges(e, m);
        source = order[(int)rnd(0, half - 1)];
        destination = order[(int)rnd(half, n - 1)];
    }
    put_int(n);
    put_int_grid(e, m, 2);
    put_int(source);
    put_int(destination);
    free(order);
    free(e);
}
