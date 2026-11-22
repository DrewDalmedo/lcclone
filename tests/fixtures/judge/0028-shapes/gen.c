void gen(int small)
{
    static const int keys[] = {5, 3, 8, 1, 4, 9, 7};
    static const int parent[] = {-1, 0, 0, 1, 1, 3};
    static const int val[] = {1, 2, 3, 4, 5, 6};
    gen_tree *a = tree_new(), *b = tree_new(), *c = tree_new(),
             *d = tree_new();
    int order[8], i;

    tree_complete(a, 7);
    for (i = 0; i < a->n; i++)
        a->val[i] = i + 1;
    tree_chain(b, 4, TREE_ZIGZAG);
    for (i = 0; i < b->n; i++)
        b->val[i] = 10 * i;
    for (i = 0; i < 7; i++)
        tree_bst_insert(c, keys[i]);
    tree_random(d, small ? 6 : 8);
    tree_inorder(d, order);
    for (i = 0; i < d->n; i++)
        d->val[order[i]] = i;
    put_tree(a);
    put_tree(b);
    put_tree(c);
    put_tree(d);
    put_nary(val, parent, 6);
    put_list(val, 6);
    put_int(tree_depth(c));
    tree_free(a);
    tree_free(b);
    tree_free(c);
    tree_free(d);
}
