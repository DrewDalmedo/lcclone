/* A number in array form and a k. The seed picks the shape: random
 * digits, all 9s (the carry runs off the front), or random digits ending
 * in a run of 9s (the carry stops partway up). k is sometimes small,
 * sometimes up to 10^4, and now and then exactly 10^4. Small cases are a
 * few digits long, so k often has more digits than num. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 6) : 10000, i;
    int shape = (int)rnd(0, 3);
    int kshape = (int)rnd(0, 3);
    int nines, k;
    int *num = malloc((size_t)n * sizeof *num);

    for (i = 0; i < n; i++)
        num[i] = (int)rnd(0, 9);
    if (shape == 2) {
        for (i = 0; i < n; i++)
            num[i] = 9;
    } else if (shape == 3 && n > 1) {
        nines = (int)rnd(1, n - 1);
        for (i = n - nines; i < n; i++)
            num[i] = 9;
    }
    if (n > 1 && num[0] == 0)
        num[0] = (int)rnd(1, 9);
    if (kshape == 0)
        k = (int)rnd(1, 20);
    else if (kshape == 1)
        k = 10000;
    else
        k = (int)rnd(1, 10000);
    put_ints(num, n);
    put_int(k);
    free(num);
}
