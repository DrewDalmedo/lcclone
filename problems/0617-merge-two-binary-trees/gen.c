/* Two binary trees, either of which may be empty. At full size, 2000 nodes
 * each unless noted: seed 1 is two paths of left children with every value
 * 10^4, 2 two paths of right children with every value -10^4, 3 a left path
 * and a right path (they share only the root, so the result has 3999
 * nodes), 4 two zigzags, 5 two random shapes, 6 two complete trees, 7 a left
 * path and an empty tree, 8 an empty tree and a right path, 9 a lone root
 * and a zigzag, and 10 and 11 two combs, a spine going right or left with a
 * leaf on the other side of each step: they fill an explicit stack of pairs. */
static void set_values(gen_tree *t, int lo, int hi)
{
    int i;

    for (i = 0; i < t->n; i++)
        t->val[i] = (int)rnd(lo, hi);
}

static void comb(gen_tree *t, int n, int side)
{
    int spine = tree_add(t, -1, 0, 0), added = 1;

    while (added < n) {
        int next = tree_add(t, spine, side, 0);

        added++;
        if (added < n) {
            tree_add(t, spine, !side, 0);
            added++;
        }
        spine = next;
    }
}

void gen(int small)
{
    gen_tree *a = tree_new(), *b = tree_new();

    if (small) {
        int na = (int)rnd(0, 6);
        int nb = (int)rnd(0, 6);

        tree_random(a, na);
        tree_random(b, nb);
        set_values(a, -3, 3);
        set_values(b, -3, 3);
    } else {
        int shape = (int)((gen_seed() - 1) % 11);
        int n = 2000;

        if (shape == 0 || shape == 6 || shape == 2)
            tree_chain(a, n, TREE_LEFT);
        else if (shape == 1)
            tree_chain(a, n, TREE_RIGHT);
        else if (shape == 3)
            tree_chain(a, n, TREE_ZIGZAG);
        else if (shape == 4)
            tree_random(a, n);
        else if (shape == 5)
            tree_complete(a, n);
        else if (shape == 8)
            tree_random(a, 1);
        else if (shape == 9)
            comb(a, n, 1);
        else if (shape == 10)
            comb(a, n, 0);

        if (shape == 0)
            tree_chain(b, n, TREE_LEFT);
        else if (shape == 1 || shape == 2 || shape == 7)
            tree_chain(b, n, TREE_RIGHT);
        else if (shape == 3 || shape == 8)
            tree_chain(b, n, TREE_ZIGZAG);
        else if (shape == 4)
            tree_random(b, n);
        else if (shape == 5)
            tree_complete(b, n);
        else if (shape == 9)
            comb(b, n, 1);
        else if (shape == 10)
            comb(b, n, 0);

        if (shape == 0) {
            set_values(a, 10000, 10000);
            set_values(b, 10000, 10000);
        } else if (shape == 1) {
            set_values(a, -10000, -10000);
            set_values(b, -10000, -10000);
        } else {
            set_values(a, -10000, 10000);
            set_values(b, -10000, 10000);
        }
    }
    put_tree(a);
    put_tree(b);
    tree_free(a);
    tree_free(b);
}
