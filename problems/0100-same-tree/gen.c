/* Two trees, p and q. Small: tiny trees where q is p, p with one change, or
 * another tree. At full size, 100 nodes: seeds 1 to 5 are a left path, a right
 * path, a zigzag, a random shape and a complete tree with q equal to p (true).
 * Seed 6 is a left path whose deepest value differs, 7 a right path whose last
 * node q lacks, 8 a zigzag whose last node is on the other side in q, 9 a
 * random shape with two values swapped, and 10 a complete tree against a path
 * with every value equal (all false). Seeds 11 and 12 are caterpillars down the
 * left and the right with q equal to p (true). */

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

/* n nodes: a spine down one side, each spine node with a leaf on the other. */
static void make_caterpillar(gen_tree *t, int n, int side)
{
    int cur = tree_add(t, -1, 0, 0);

    while (t->n + 2 <= n) {
        int next = tree_add(t, cur, side, 0);

        tree_add(t, cur, !side, 0);
        cur = next;
    }
    if (t->n < n)
        tree_add(t, cur, !side, 0);
}

/* Unlinks node x, a leaf, from its parent. */
static void drop_leaf(gen_tree *t, int x)
{
    int i;

    if (x == t->root) {
        t->root = -1;
        return;
    }
    for (i = 0; i < t->n; i++) {
        if (t->left[i] == x)
            t->left[i] = -1;
        if (t->right[i] == x)
            t->right[i] = -1;
    }
}

static void gen_small(void)
{
    gen_tree *t = tree_new();
    int n = (int)rnd(0, 6), shape = (int)rnd(0, 4), kind = (int)rnd(0, 4), i;

    make_shape(t, n, shape);
    for (i = 0; i < n; i++)
        t->val[i] = (int)rnd(-2, 2);
    put_tree(t);
    if (kind == 1 && n > 0) {
        int who = (int)rnd(0, n - 1);

        t->val[who] = t->val[who] == 2 ? 1 : t->val[who] + 1;
    } else if (kind == 2 && n > 0) {
        int who = (int)rnd(0, n - 1), c = t->left[who];

        t->left[who] = t->right[who];
        t->right[who] = c;
    } else if (kind == 3 && n > 0) {
        drop_leaf(t, n - 1); /* the last node added has no children */
    } else if (kind == 4) {
        int m = (int)rnd(0, 6), other = (int)rnd(0, 4);

        tree_free(t);
        t = tree_new();
        make_shape(t, m, other);
        for (i = 0; i < m; i++)
            t->val[i] = (int)rnd(-2, 2);
    }
    put_tree(t);
    tree_free(t);
}

static void gen_full(int s)
{
    gen_tree *t = tree_new();
    int n = 100, a = 0, b = 0, i;

    if (s >= 10)
        make_caterpillar(t, n, s - 10);
    else
        make_shape(t, n, s < 5 ? s : s - 5);
    for (i = 0; i < n; i++)
        t->val[i] = (int)rnd(-10000, 10000);
    t->val[0] = -10000;
    t->val[n - 1] = 10000;
    if (s == 8) {
        a = (int)rnd(0, n - 1);
        b = (int)rnd(0, n - 2);
        b += b >= a;
        t->val[a] = 10000;
        t->val[b] = -10000;
    } else if (s == 9) {
        for (i = 0; i < n; i++)
            t->val[i] = 10000;
    }
    put_tree(t);
    if (s == 5) {
        t->val[n - 1] = 9999;
    } else if (s == 6) {
        drop_leaf(t, n - 1);
    } else if (s == 7) {
        t->right[n - 2] = -1; /* a zigzag's node 99 is a right child */
        t->left[n - 2] = n - 1;
    } else if (s == 8) {
        t->val[a] = -10000;
        t->val[b] = 10000;
    } else if (s == 9) {
        tree_free(t);
        t = tree_new();
        tree_chain(t, n, TREE_RIGHT);
        for (i = 0; i < n; i++)
            t->val[i] = 10000;
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
