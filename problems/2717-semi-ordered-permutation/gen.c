/* A random permutation of 1 to n. Small cases have n from 2 to 8. Full-size
 * ones have n = 50, and the seed picks the shape: random, n first and 1
 * last (the most swaps), or 1 first and n last (no swaps), with the other
 * books shuffled. */
static void swap_to(int *a, int n, int value, int pos)
{
    int i, t;

    for (i = 0; i < n; i++)
        if (a[i] == value) {
            t = a[pos];
            a[pos] = a[i];
            a[i] = t;
            return;
        }
}

void gen(int small)
{
    int a[50], n = small ? (int)rnd(2, 8) : 50, i, shape;

    for (i = 0; i < n; i++)
        a[i] = i + 1;
    rnd_shuffle(a, n);
    if (!small) {
        shape = (int)rnd(0, 3);
        if (shape == 2) {
            swap_to(a, n, n, 0);
            swap_to(a, n, 1, n - 1);
        } else if (shape == 3) {
            swap_to(a, n, 1, 0);
            swap_to(a, n, n, n - 1);
        }
    }
    put_ints(a, n);
}
