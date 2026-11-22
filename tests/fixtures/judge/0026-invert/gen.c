void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(0, 9) : 100000, i;

    if (small ? rnd_chance(50) : gen_seed() % 2 == 1)
        tree_random(t, n);
    else
        tree_chain(t, n, TREE_ZIGZAG);
    for (i = 0; i < t->n; i++)
        t->val[i] = (int)rnd(-100, 100);
    put_tree(t);
    tree_free(t);
}
