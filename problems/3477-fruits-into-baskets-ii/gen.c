/* Batch sizes and basket capacities, in a shape the seed picks:
 * - random sizes and capacities;
 * - the same, with the baskets sorted by capacity, so most batches scan
 *   far along the row;
 * - capacities only up to half the largest size, so many batches are
 *   left out after scanning the whole row;
 * - everything one of two neighbouring values, so a batch often exactly
 *   fills a basket.
 * Small cases have 1 to 8 batches with values up to 10 at most;
 * full-size ones have 100 with values up to 1000. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, i, j, t, v;
    int top = small ? (int)rnd(2, 10) : 1000, shape = (int)rnd(0, 3);
    int *fruits = malloc((size_t)n * sizeof *fruits);
    int *baskets = malloc((size_t)n * sizeof *baskets);

    v = (int)rnd(1, top - 1);
    for (i = 0; i < n; i++)
        fruits[i] = shape == 3 ? v + (int)rnd(0, 1) : (int)rnd(1, top);
    for (j = 0; j < n; j++) {
        if (shape == 3)
            baskets[j] = v + (int)rnd(0, 1);
        else if (shape == 2)
            baskets[j] = (int)rnd(1, top / 2);
        else
            baskets[j] = (int)rnd(1, top);
    }
    if (shape == 1)
        for (i = 1; i < n; i++) {
            t = baskets[i];
            for (j = i; j > 0 && baskets[j - 1] > t; j--)
                baskets[j] = baskets[j - 1];
            baskets[j] = t;
        }
    put_ints(fruits, n);
    put_ints(baskets, n);
    free(fruits);
    free(baskets);
}
