/* Digits. At full size: seed 1 is a palindrome of 10^5, 2 of 99999, 3 is a
 * palindrome of 10^5 with one of the two middle digits changed (the check
 * from both ends finds it last), 4 is 10^5 equal digits, and 5 is equal
 * digits but for the last. */
void gen(int small)
{
    int n, i, shape = small ? -1 : (int)((gen_seed() - 1) % 5);
    int *a;

    n = small ? (int)rnd(1, 7) : shape == 1 ? 99999 : 100000;
    a = malloc((size_t)n * sizeof *a);
    if (small) {
        int mirror = rnd_chance(60);

        for (i = 0; i < n; i++)
            a[i] = (int)rnd(0, 2);
        if (mirror)
            for (i = 0; i < n / 2; i++)
                a[n - 1 - i] = a[i];
    } else if (shape <= 2) {
        for (i = 0; i < n; i++)
            a[i] = (int)rnd(0, 9);
        for (i = 0; i < n / 2; i++)
            a[n - 1 - i] = a[i];
        if (shape == 2)
            a[n / 2] = (a[n / 2 - 1] + 1) % 10;
    } else {
        for (i = 0; i < n; i++)
            a[i] = 7;
        if (shape == 4)
            a[n - 1] = 8;
    }
    put_list(a, n);
    free(a);
}
