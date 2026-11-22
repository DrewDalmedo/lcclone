/* A list. At full size, 5000 nodes: seed 1 is random, 2 increases across
 * the whole range, 3 is all -5000, and 4 decreases from 5000. */
void gen(int small)
{
    int n = small ? (int)rnd(0, 7) : 5000, i;
    int shape = small ? -1 : (int)((gen_seed() - 1) % 4);
    int *a = malloc(((size_t)n + 1) * sizeof *a);

    for (i = 0; i < n; i++) {
        if (small)
            a[i] = (int)rnd(-3, 3);
        else if (shape == 0)
            a[i] = (int)rnd(-5000, 5000);
        else if (shape == 1)
            a[i] = -5000 + 2 * i;
        else if (shape == 2)
            a[i] = -5000;
        else
            a[i] = 5000 - 2 * i;
    }
    put_list(a, n);
    free(a);
}
