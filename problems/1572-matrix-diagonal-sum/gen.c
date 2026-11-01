/* A random square grid. Small cases have sides 1 to 7, odd and even, with
 * values up to 9 or up to 100; full-size ones have side 99 or 100, and are
 * sometimes all 100s, the largest total for that side. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 7) : (int)rnd(99, 100), i;
    int top = small ? (rnd_chance(50) ? 9 : 100) : 100;
    int all_top = !small && rnd_chance(25);
    int *a = malloc((size_t)n * n * sizeof *a);

    for (i = 0; i < n * n; i++)
        a[i] = all_top ? top : (int)rnd(1, top);
    put_int_grid(a, n, n);
    free(a);
}
