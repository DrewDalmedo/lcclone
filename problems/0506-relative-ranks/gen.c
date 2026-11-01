/* Distinct scores. The seed picks how they are drawn: from 0 to 20; from
 * the whole range, 0 and 10^6 included; or as a small number plus a
 * multiple of 65536, so that their low 16 bits are in a different order
 * from the scores. Small cases have up to 8 scores. Full-size ones have
 * 10^4 (even seeds) or a random number of them, in random, increasing or
 * decreasing order. */
static unsigned char used[1000001];

static int draw(int mode)
{
    long long v;

    if (mode == 0)
        return (int)rnd(0, 20);
    if (mode == 1)
        return (int)rnd(0, 1000000);
    v = rnd(0, 3000);
    return (int)(v + 65536 * rnd(0, 15));
}

static int ascending(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

void gen(int small)
{
    int n, mode, order, i, v;
    int *score;

    if (small) {
        n = (int)rnd(1, 8);
        mode = (int)rnd(0, 2);
        order = 0;
    } else {
        n = gen_seed() % 2 == 0 ? 10000 : (int)rnd(1, 10000);
        mode = (int)rnd(1, 2);
        order = (int)rnd(0, 2);
    }
    score = malloc((size_t)n * sizeof *score);
    memset(used, 0, sizeof used);
    for (i = 0; i < n; i++) {
        if (mode == 1 && i < 2 && rnd_chance(small ? 30 : 100)) {
            v = i == 0 ? 0 : 1000000;
        } else {
            do
                v = draw(mode);
            while (used[v]);
        }
        if (used[v]) {
            i--;
            continue;
        }
        used[v] = 1;
        score[i] = v;
    }
    if (order == 0) {
        rnd_shuffle(score, n);
    } else {
        qsort(score, (size_t)n, sizeof *score, ascending);
        if (order == 2)
            for (i = 0; i < n / 2; i++) {
                v = score[i];
                score[i] = score[n - 1 - i];
                score[n - 1 - i] = v;
            }
    }
    put_ints(score, n);
    free(score);
}
