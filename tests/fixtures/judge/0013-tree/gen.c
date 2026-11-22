/* At full size, 100000 nodes: a random shape for seed 1, a path of left
 * children for seed 2, right children for 3, and either at random for 4. */
void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(0, 9) : 100000, i;
    int shape = small ? (int)rnd(0, 4) : (int)(gen_seed() - 1) % 4;

    if (shape == 0 || shape == 4)
        tree_random(t, n);
    else
        tree_chain(t, n, shape == 1 ? TREE_LEFT : shape == 2 ? TREE_RIGHT
                                                              : TREE_ANY);
    for (i = 0; i < t->n; i++)
        t->val[i] = (int)rnd(-1000, 1000);
    put_tree(t);
    tree_free(t);
}
