/* Random heights. Small cases use few distinct values, so plateaus and
 * equal neighbours come up often; large ones use the whole range. */
void gen(int small)
{
    int n = small ? (int)rnd(3, 9) : 100, i;
    int top = small ? (int)rnd(2, 6) : 100;
    int *mountain = malloc((size_t)n * sizeof *mountain);

    for (i = 0; i < n; i++)
        mountain[i] = (int)rnd(1, top);
    put_ints(mountain, n);
    free(mountain);
}
