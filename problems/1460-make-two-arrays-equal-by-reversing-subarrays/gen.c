/* Two arrays of the same length. Small cases have 1 to 7 elements drawn
 * from a pool of 1 to 3 values (now and then 1 and 1000); full-size ones
 * have 1000 elements, either all different or drawn from 3 values. arr is
 * then target shuffled (true), target shuffled with one element replaced
 * by another value (false, often with the same distinct values; at full
 * size the last element, so a match is missed only at the far end), or
 * drawn on its own from the same pool. */
static int pick(const int *pool, int np, int top)
{
    int j = (int)rnd(0, np);

    return j < np ? pool[j] : (int)rnd(1, top);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 7) : 1000, top = small ? 9 : 1000;
    int pool[3], np, shape, distinct, i, k, v;
    int *target = malloc((size_t)n * sizeof *target);
    int *arr = malloc((size_t)n * sizeof *arr);

    np = (int)rnd(1, 3);
    for (i = 0; i < np; i++)
        pool[i] = (int)rnd(1, top);
    if (small && rnd_chance(25)) {
        pool[0] = 1;
        pool[1] = 1000;
        np = 2;
    }
    distinct = !small && rnd_chance(50);
    for (i = 0; i < n; i++) {
        if (distinct) {
            target[i] = i + 1;
        } else {
            k = (int)rnd(0, np - 1);
            target[i] = pool[k];
        }
    }
    rnd_shuffle(target, n);

    shape = (int)rnd(0, 2);
    if (shape < 2 || distinct) {
        memcpy(arr, target, (size_t)n * sizeof *arr);
        rnd_shuffle(arr, n);
        if (shape >= 1) {
            k = small ? (int)rnd(0, n - 1) : n - 1;
            do {
                v = distinct ? (int)rnd(1, 1000) : pick(pool, np, top);
            } while (v == arr[k]);
            arr[k] = v;
        }
    } else {
        for (i = 0; i < n; i++) {
            k = (int)rnd(0, np - 1);
            arr[i] = pool[k];
        }
    }
    put_ints(target, n);
    put_ints(arr, n);
    free(target);
    free(arr);
}
