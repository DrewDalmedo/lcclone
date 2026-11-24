/* A binary tree. Small: tiny, of one value, or with one odd node, or with two
 * deviations that cancel out, or of random values. At full size, 100 nodes:
 * seed 1 is a left path of 99s (true), 2 a right path whose deepest value is
 * off, 3 a zigzag of 0s (true), 4 a random shape of one value (true), 5 a
 * complete tree whose last node is off, 6 a random shape of 0s with one other
 * value, 7 a random shape of 9s with one 0, 8 a complete tree with a +1 and a
 * -1 that keep the sum, 9 a comb whose deepest node, in the root's right
 * subtree, is off, 10 a comb of one value (true), 11 a comb going left of one
 * value (true), and 12 a comb going left whose deepest node is off. */

static void make_shape(gen_tree *t, int n, int shape)
{
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
}

/* n nodes: a spine going down side, each spine node with a leaf on the other
 * side. */
static void make_comb(gen_tree *t, int n, int side)
{
    int cur = tree_add(t, -1, 0, 0);

    while (t->n + 2 <= n) {
        tree_add(t, cur, !side, 0);
        cur = tree_add(t, cur, side, 0);
    }
    if (t->n < n)
        tree_add(t, cur, !side, 0);
}

static void fill_all(gen_tree *t, int v)
{
    int i;

    for (i = 0; i < t->n; i++)
        t->val[i] = v;
}

static void gen_small(void)
{
    gen_tree *t = tree_new();
    int n = (int)rnd(1, 7), shape = (int)rnd(0, 4), kind = (int)rnd(0, 3);
    int v = (int)rnd(0, 2), i;

    make_shape(t, n, shape);
    fill_all(t, v);
    if (kind == 1) {
        int who = (int)rnd(0, n - 1);

        t->val[who] = v == 2 ? 0 : v + 1;
    } else if (kind == 2 && n >= 2) {
        int a = (int)rnd(0, n - 1), b = (int)rnd(0, n - 2);

        b += b >= a;
        fill_all(t, 1);
        t->val[a] = 2;
        t->val[b] = 0;
    } else if (kind == 3) {
        for (i = 0; i < n; i++)
            t->val[i] = (int)rnd(0, 1);
    }
    put_tree(t);
    tree_free(t);
}

static void gen_full(int s)
{
    gen_tree *t = tree_new();
    int n = 100;

    if (s >= 8)
        make_comb(t, n, s < 10);
    else if (s == 3 || s == 5 || s == 6)
        tree_random(t, n);
    else if (s == 4 || s == 7)
        tree_complete(t, n);
    else
        tree_chain(t, n, s == 0 ? TREE_LEFT : s == 1 ? TREE_RIGHT : TREE_ZIGZAG);
    if (s == 0) {
        fill_all(t, 99);
    } else if (s == 1) {
        fill_all(t, 7);
        t->val[n - 1] = 8;
    } else if (s == 2) {
        fill_all(t, 0);
    } else if (s == 3) {
        fill_all(t, (int)rnd(0, 99));
    } else if (s == 4) {
        fill_all(t, 5);
        t->val[n - 1] = 6;
    } else if (s == 5) {
        int who = (int)rnd(0, n - 1), v = (int)rnd(1, 99);

        fill_all(t, 0);
        t->val[who] = v;
    } else if (s == 6) {
        int who = (int)rnd(0, n - 1);

        fill_all(t, 9);
        t->val[who] = 0;
    } else if (s == 7) {
        int a = (int)rnd(0, n - 1), b = (int)rnd(0, n - 2);

        b += b >= a;
        fill_all(t, 50);
        t->val[a] = 51;
        t->val[b] = 49;
    } else if (s == 8) {
        fill_all(t, 12);
        t->val[n - 1] = 13; /* the last node added is the deepest */
    } else if (s == 10) {
        fill_all(t, 33);
    } else if (s == 11) {
        fill_all(t, 44);
        t->val[n - 1] = 45;
    } else {
        fill_all(t, 1);
    }
    put_tree(t);
    tree_free(t);
}

void gen(int small)
{
    if (small)
        gen_small();
    else
        gen_full((int)((gen_seed() - 1) % 12));
}
