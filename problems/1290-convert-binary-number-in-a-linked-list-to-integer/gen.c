/* Bits. At full size, 30 nodes: seed 1 is all ones (the largest answer), 2
 * is random with a leading one, 3 is a one and 29 zeros, and 4 is all
 * zeros. */
void gen(int small)
{
    int a[30], n = small ? (int)rnd(1, 6) : 30, i;
    int shape = small ? -1 : (int)((gen_seed() - 1) % 4);

    for (i = 0; i < n; i++) {
        if (small || shape == 1)
            a[i] = (int)rnd(0, 1);
        else
            a[i] = shape == 0;
    }
    if (shape == 1 || shape == 2)
        a[0] = 1;
    put_list(a, n);
}
