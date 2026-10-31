/* Random heights and threshold. Small cases draw both from a few values,
 * so heights equal to the threshold come up often. */
void gen(int small)
{
    int top = small ? (int)rnd(2, 6) : 100;
    int threshold = (int)rnd(1, top);
    int n = small ? (int)rnd(2, 8) : 100, i;
    int *height = malloc((size_t)n * sizeof *height);

    for (i = 0; i < n; i++)
        height[i] = (int)rnd(1, top);
    put_ints(height, n);
    put_int(threshold);
    free(height);
}
