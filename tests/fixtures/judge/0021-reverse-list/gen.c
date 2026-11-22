void gen(int small)
{
    int n = small ? (int)rnd(0, 8) : 100000, i;
    int *vals = malloc(((size_t)n + 1) * sizeof *vals);

    for (i = 0; i < n; i++)
        vals[i] = (int)rnd(-5000, 5000);
    put_list(vals, n);
    free(vals);
}
