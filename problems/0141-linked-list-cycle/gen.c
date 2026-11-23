/* A list and where its last node links back to. At full size, 10^4 nodes:
 * seed 1 has no cycle, 2 links back to the head (the longest loop), 3 is
 * the last node linked to itself, 4 links to a random node, 5 has no cycle
 * and all values equal, and 6 links to the middle with all values equal. */
void gen(int small)
{
    int n = small ? (int)rnd(0, 6) : 10000, i, pos = -1;
    int shape = small ? -1 : (int)((gen_seed() - 1) % 6);
    int *a = malloc(((size_t)n + 1) * sizeof *a);

    for (i = 0; i < n; i++) {
        if (small)
            a[i] = (int)rnd(-2, 2);
        else if (shape >= 4)
            a[i] = 100000;
        else
            a[i] = (int)rnd(-100000, 100000);
    }
    if (!small && shape < 4) {
        a[0] = -100000;
        a[n - 1] = 100000;
    }
    if (small) {
        if (n > 0 && rnd_chance(60))
            pos = (int)rnd(0, n - 1);
    } else if (shape == 1) {
        pos = 0;
    } else if (shape == 2) {
        pos = n - 1;
    } else if (shape == 3) {
        pos = (int)rnd(1, n - 2);
    } else if (shape == 5) {
        pos = n / 2;
    }
    put_list(a, n);
    put_int(pos);
    free(a);
}
