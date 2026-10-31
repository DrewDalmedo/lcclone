/* Random digits with plenty of zeros: each element is zero with a chance
 * picked per case, from none to all, and otherwise a digit from 1 to 9
 * (only 1 to 3 in a small case, so that values repeat). */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 10000, i;
    int zeros = (int)rnd(0, 100);
    int *arr = malloc((size_t)n * sizeof *arr);

    for (i = 0; i < n; i++)
        arr[i] = rnd_chance(zeros) ? 0 : (int)rnd(1, small ? 3 : 9);
    put_ints(arr, n);
    free(arr);
}
