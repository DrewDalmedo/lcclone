/* A star: the center joined to every other node, the edges in random
 * order. Within the pairs the center is always first, always second, or
 * either one at random, as the seed picks. Small cases have 3 to 8 nodes
 * and any center. Full-size cases have n = 10^5; the seed makes the
 * center n (last to be tried by a search over the nodes), 1, a node near
 * n, or any node. */
void gen(int small)
{
    int n = small ? (int)rnd(3, 8) : 100000;
    int shape = (int)rnd(0, 3), orient = (int)rnd(0, 2), m = n - 1;
    int *leaf = malloc((size_t)m * sizeof *leaf);
    int *out = malloc((size_t)m * 2 * sizeof *out);
    int c, i, k, first;

    if (small || shape == 3)
        c = (int)rnd(1, n);
    else if (shape == 0)
        c = n;
    else if (shape == 1)
        c = 1;
    else
        c = (int)rnd(n - 100, n - 1);
    for (i = 1, k = 0; i <= n; i++)
        if (i != c)
            leaf[k++] = i;
    rnd_shuffle(leaf, m);
    for (i = 0; i < m; i++) {
        first = orient == 0 || (orient == 2 && rnd_chance(50));
        out[2 * i] = first ? c : leaf[i];
        out[2 * i + 1] = first ? leaf[i] : c;
    }
    put_int_grid(out, m, 2);
    free(out);
    free(leaf);
}
