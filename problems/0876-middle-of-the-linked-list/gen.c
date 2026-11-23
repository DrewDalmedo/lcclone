/* A list. At full size: seed 1 is 100 distinct values, 2 is 99 random
 * values, and 3 is 100 equal values, where only the length of the output
 * tells the nodes apart. */
void gen(int small)
{
    int a[100], n, i, shape = small ? -1 : (int)((gen_seed() - 1) % 3);

    n = small ? (int)rnd(1, 8) : shape == 1 ? 99 : 100;
    for (i = 0; i < n; i++) {
        if (small)
            a[i] = (int)rnd(1, 4);
        else if (shape == 0)
            a[i] = 100 - i;
        else if (shape == 1)
            a[i] = (int)rnd(1, 100);
        else
            a[i] = 100;
    }
    put_list(a, n);
}
