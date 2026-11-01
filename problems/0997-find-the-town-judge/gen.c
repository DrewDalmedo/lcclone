/* n people and a list of distinct pairs [a, b] with a != b, in random
 * order. The seed picks the shape: a judge, a near-judge (trusted by all
 * others but one, or trusted by all but trusting one person), or just
 * random pairs. In the first three, the extra random pairs never start at
 * the (near-)judge. Small cases have a few people and pairs; full-size
 * ones have n = 1000 and 10^4 pairs. */
static unsigned char *seen;
static int *pairs, npairs, people;

/* Adds [a, b] unless it is already there. */
static void add(int a, int b)
{
    if (seen[(a - 1) * people + (b - 1)])
        return;
    seen[(a - 1) * people + (b - 1)] = 1;
    pairs[2 * npairs] = a;
    pairs[2 * npairs + 1] = b;
    npairs++;
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 6) : 1000;
    int shape = (int)rnd(0, 3);
    int j = (int)rnd(1, n);
    int most = n * (n - 1) < 10000 ? n * (n - 1) : 10000;
    int skip = 0, i, a, b, c, ncand = 0, extra;
    int *cand, *order, *out;

    people = n;
    seen = calloc((size_t)n * n, 1);
    pairs = malloc((size_t)(most + 1) * 2 * sizeof *pairs);
    npairs = 0;
    if (n > 1 && shape <= 2) {
        /* Everyone but j trusts j; a near-judge misses one of these
         * pairs, or trusts someone itself. */
        if (shape == 1) {
            skip = (int)rnd(1, n - 1);
            if (skip >= j)
                skip++;
        }
        for (i = 1; i <= n; i++)
            if (i != j && i != skip)
                add(i, j);
        if (shape == 2) {
            b = (int)rnd(1, n - 1);
            if (b >= j)
                b++;
            add(j, b);
        }
    }
    if (small) {
        /* Every pair still allowed, in random order; take some of them. */
        cand = malloc((size_t)(n * n + 1) * sizeof *cand);
        for (a = 1; a <= n; a++)
            for (b = 1; b <= n; b++) {
                if (a == b || seen[(a - 1) * n + (b - 1)])
                    continue;
                if (shape <= 2 && (a == j || (a == skip && b == j)))
                    continue;
                cand[ncand++] = (a - 1) * n + (b - 1);
            }
        rnd_shuffle(cand, ncand);
        extra = (int)rnd(0, ncand < 6 ? ncand : 6);
        for (i = 0; i < extra; i++)
            add(cand[i] / n + 1, cand[i] % n + 1);
        free(cand);
    } else {
        while (npairs < most) {
            a = (int)rnd(1, n);
            c = (int)rnd(1, n - 1);
            b = c >= a ? c + 1 : c;
            if (shape <= 2 && (a == j || (a == skip && b == j)))
                continue;
            add(a, b);
        }
    }
    order = malloc((size_t)(npairs + 1) * sizeof *order);
    out = malloc((size_t)(npairs + 1) * 2 * sizeof *out);
    for (i = 0; i < npairs; i++)
        order[i] = i;
    rnd_shuffle(order, npairs);
    for (i = 0; i < npairs; i++) {
        out[2 * i] = pairs[2 * order[i]];
        out[2 * i + 1] = pairs[2 * order[i] + 1];
    }
    put_int(n);
    put_int_grid(out, npairs, 2);
    free(out);
    free(order);
    free(pairs);
    free(seen);
}
