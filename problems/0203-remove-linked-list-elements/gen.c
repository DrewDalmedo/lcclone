/* A list and a value to cut. At full size, 10^4 nodes: seed 1 is random, 2
 * cuts everything, 3 cuts nothing (val 0), 4 cuts every other node, 5 cuts
 * long runs at both ends, and 6 cuts only the last node. */
void gen(int small)
{
    int n = small ? (int)rnd(0, 7) : 10000, i, val;
    int shape = small ? -1 : (int)((gen_seed() - 1) % 6);
    int *a = malloc(((size_t)n + 1) * sizeof *a);

    if (small) {
        val = (int)rnd(0, 3);
        for (i = 0; i < n; i++)
            a[i] = (int)rnd(1, 3);
    } else {
        val = shape == 0 ? (int)rnd(1, 50) : shape == 2 ? 0 : 50;
        for (i = 0; i < n; i++) {
            int v = (int)rnd(1, 49);

            if (shape == 1 || (shape == 3 && i % 2 == 0) ||
                (shape == 4 && (i < 3000 || i >= n - 3000)) ||
                (shape == 5 && i == n - 1))
                v = 50;
            a[i] = v;
        }
    }
    put_list(a, n);
    put_int(val);
    free(a);
}
