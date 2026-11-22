/* Node i's parent is one of the nodes before it: any of them (wide), or
 * the one right before (a path). */
void gen(int small)
{
    int n = small ? (int)rnd(0, 9) : 10000, i;
    int path = small ? rnd_chance(30) : gen_seed() % 2 == 0;
    int *val = malloc(((size_t)n + 1) * sizeof *val);
    int *parent = malloc(((size_t)n + 1) * sizeof *parent);

    for (i = 0; i < n; i++) {
        val[i] = (int)rnd(0, 10000);
        parent[i] = i == 0 ? -1 : path ? i - 1 : (int)rnd(0, i - 1);
    }
    put_nary(val, parent, n);
    free(val);
    free(parent);
}
