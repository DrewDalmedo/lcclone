/* Random values. Small cases have 20, 40 or 60 elements from a narrow
 * range, so the smallest and largest values repeat, sometimes more than
 * n / 20 times; now and then a few copies of 0 or 10^5 are mixed in. */
void gen(int small)
{
    int n = small ? 20 * (int)rnd(1, 3) : 1000, i;
    int top = small ? (int)rnd(0, 9) : 100000;
    int *arr = malloc((size_t)n * sizeof *arr);

    for (i = 0; i < n; i++)
        arr[i] = (int)rnd(0, top);
    if (small && rnd_chance(30))
        for (i = (int)rnd(0, 5); i > 0; i--)
            arr[rnd(0, n - 1)] = rnd_chance(50) ? 0 : 100000;
    put_ints(arr, n);
    free(arr);
}
