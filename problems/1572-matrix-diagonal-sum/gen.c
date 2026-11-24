/* A random square grid. Small cases have sides 1 to 7, odd and even, with
 * values up to 9 or up to 100; full-size ones have side 99 (odd seeds) or
 * 100 (even ones), and are all 100s, the largest total for that side, when
 * gen_seed() % 4 is 2 or 3. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 7) : 99 + (int)(gen_seed() % 2 == 0), i;
    int top = small ? (rnd_chance(50) ? 9 : 100) : 100;
    int all_top = !small && gen_seed() % 4 >= 2;
    int *a = malloc((size_t)n * n * sizeof *a);

    for (i = 0; i < n * n; i++)
        a[i] = all_top ? top : (int)rnd(1, top);
    put_int_grid(a, n, n);
    free(a);
}
