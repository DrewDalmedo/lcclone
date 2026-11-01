/* Random values. Small cases have 1 to 9 elements, odd and even counts,
 * with values up to 10 or up to 1000; full-size ones have 99 or 100
 * elements up to 1000, and are sometimes all 1000s (100 of them give the
 * largest answer, 85850000). */
void gen(int small)
{
    int n = small ? (int)rnd(1, 9) : (int)rnd(99, 100), i;
    int top = small && rnd_chance(60) ? 10 : 1000;
    int all_top = !small && rnd_chance(25);
    int *arr = malloc((size_t)n * sizeof *arr);

    for (i = 0; i < n; i++)
        arr[i] = all_top ? top : (int)rnd(1, top);
    put_ints(arr, n);
    free(arr);
}
