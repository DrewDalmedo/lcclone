/* A tree of distinct values and one of them. At full size, 10^4 nodes:
 * seed 1 is a path of left children, 2 of right children, 3 a zigzag (each
 * with the deepest node as target), 4 a random shape with a random target,
 * and 5 a complete tree with its last node as target. */
void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(1, 7) : 10000, i, who;
    int shape = small ? 3 : (int)((gen_seed() - 1) % 5);
    int m = small ? 9 : n;
    int *v = malloc((size_t)m * sizeof *v);

    if (shape == 0)
        tree_chain(t, n, TREE_LEFT);
    else if (shape == 1)
        tree_chain(t, n, TREE_RIGHT);
    else if (shape == 2)
        tree_chain(t, n, TREE_ZIGZAG);
    else if (shape == 3)
        tree_random(t, n);
    else
        tree_complete(t, n);
    for (i = 0; i < m; i++)
        v[i] = small ? i + 1 : (i + 1) * 10 - (int)rnd(0, 9);
    rnd_shuffle(v, m);
    for (i = 0; i < n; i++)
        t->val[i] = v[i];
    who = shape == 3 ? (int)rnd(0, n - 1) : n - 1;
    put_tree(t);
    put_int(t->val[who]);
    tree_free(t);
    free(v);
}
