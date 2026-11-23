/* An N-ary tree, as each node's parent. At full size, 10^4 nodes and at
 * most 1000 levels: seed 1 is a path of 1000 with the rest hung at random,
 * 2 a root with 9999 leaves, 3 a random tree, 4 a root whose last child
 * starts the path of 1000, and 5 a path of 1000 with leaves along it. */
void gen(int small)
{
    int n = small ? (int)rnd(0, 8) : 10000, i;
    int shape = small ? (int)rnd(0, 4) : (int)((gen_seed() - 1) % 5);
    int spine = small ? (n + 1) / 2 : 1000;
    int *val = malloc(((size_t)n + 1) * sizeof *val);
    int *parent = malloc(((size_t)n + 1) * sizeof *parent);
    int *depth = malloc(((size_t)n + 1) * sizeof *depth);

    for (i = 0; i < n; i++)
        val[i] = (int)(small ? rnd(0, 5) : rnd(0, 10000));
    for (i = 0; i < n; i++) {
        int p;

        if (i == 0) {
            parent[0] = -1;
            depth[0] = 1;
            continue;
        }
        if (shape == 1) {
            p = 0;
        } else if (shape == 2) {
            p = (int)rnd(0, i - 1);
        } else if (shape == 3) {
            int leaves = small ? n / 2 : n - 1000;

            p = i <= leaves ? 0 : i == leaves + 1 ? 0 : i - 1;
        } else if (i < spine) {
            p = i - 1;
        } else if (shape == 4) {
            p = (int)rnd(0, spine >= 2 ? spine - 2 : 0);
        } else {
            do {
                p = (int)rnd(0, i - 1);
            } while (!small && depth[p] >= 1000);
        }
        parent[i] = p;
        depth[i] = depth[p] + 1;
    }
    put_nary(val, parent, n);
    free(val);
    free(parent);
    free(depth);
}
