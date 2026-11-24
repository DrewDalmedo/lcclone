/* A binary tree. Small: a root with an arm and its reflection, sometimes with
 * one change or with a plain copy in place of the reflection, or a random
 * tree. At full size: seeds 1 to 5 have 1000 nodes (false, as an even count
 * must be): a left path, a right path, a zigzag, a random shape, and a complete
 * tree of equal values. Seeds 6 to 9 have 999 nodes and are symmetric: a root
 * over two bare arms 500 levels deep, over zigzag arms, over random arms, and
 * over complete arms with values at the bounds. Seed 10 is random arms with
 * one deep value changed, 11 zigzag arms of equal values with one leaf gone
 * (998 nodes), and 12 random arms with one leaf added (1000 nodes). Seeds 13
 * and 14 are symmetric, 999 nodes, over arms that are caterpillars. */

/* Puts a root on t, arm as its left subtree and, as its right, a copy of arm
 * that is reflected when flip is set. Node i of arm becomes nodes 2i + 1 (left
 * side) and 2i + 2 (right side) of t. */
static void add_arms(gen_tree *t, const gen_tree *arm, int root_val, int flip)
{
    int m = arm->n, i;
    int *up = malloc((size_t)(m + 1) * sizeof *up);
    int *way = malloc((size_t)(m + 1) * sizeof *way);

    for (i = 0; i < m; i++) {
        int l = arm->left[i], r = arm->right[i];

        if (l >= 0) {
            up[l] = i;
            way[l] = 0;
        }
        if (r >= 0) {
            up[r] = i;
            way[r] = 1;
        }
    }
    tree_add(t, -1, 0, root_val);
    for (i = 0; i < m; i++) {
        if (i == 0) {
            tree_add(t, 0, 0, arm->val[0]);
            tree_add(t, 0, 1, arm->val[0]);
        } else {
            tree_add(t, 2 * up[i] + 1, way[i], arm->val[i]);
            tree_add(t, 2 * up[i] + 2, flip ? !way[i] : way[i], arm->val[i]);
        }
    }
    free(up);
    free(way);
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

    for (i = 0; i < t->n; i++) {
        if (t->left[i] == x)
            t->left[i] = -1;
        if (t->right[i] == x)
            t->right[i] = -1;
    }
}

static void gen_small(void)
{
    gen_tree *t = tree_new(), *arm = tree_new();
    int kind = (int)rnd(0, 4), i;

    if (kind == 4) {
        int n = (int)rnd(1, 7);

        tree_random(t, n);
        for (i = 0; i < n; i++)
            t->val[i] = (int)rnd(0, 1);
    } else {
        int m = (int)rnd(0, 3), how = (int)rnd(0, 1), top;

        if (how)
            tree_random(arm, m);
        else
            tree_chain(arm, m, TREE_ANY);
        for (i = 0; i < m; i++)
            arm->val[i] = (int)rnd(-1, 1);
        top = (int)rnd(-1, 1);
        add_arms(t, arm, top, kind != 3);
        if (kind == 1 && m > 0) {
            int who = 2 * (int)rnd(0, m - 1) + 1;

            t->val[who] = t->val[who] == 1 ? 0 : t->val[who] + 1;
        } else if (kind == 2 && m > 0) {
            drop_leaf(t, 2 * m); /* the arm's last node has no children */
        }
    }
    put_tree(t);
    tree_free(t);
    tree_free(arm);
}

static void gen_full(int s)
{
    gen_tree *t = tree_new(), *arm = tree_new();
    int m = 499, n = 1000, i, top;

    if (s < 5) {
        if (s == 0)
            tree_chain(t, n, TREE_LEFT);
        else if (s == 1)
            tree_chain(t, n, TREE_RIGHT);
        else if (s == 2)
            tree_chain(t, n, TREE_ZIGZAG);
        else if (s == 3)
            tree_random(t, n);
        else
            tree_complete(t, n);
        for (i = 0; i < n; i++)
            t->val[i] = s == 4 ? 100 : (int)rnd(-100, 100);
    } else {
        if (s == 5)
            tree_chain(arm, m, TREE_LEFT);
        else if (s == 6 || s == 10)
            tree_chain(arm, m, TREE_ZIGZAG);
        else if (s == 8)
            tree_complete(arm, m);
        else if (s >= 12)
            make_caterpillar(arm, m, s - 12);
        else
            tree_random(arm, m);
        for (i = 0; i < m; i++) {
            if (s == 8)
                arm->val[i] = i % 2 ? 100 : -100;
            else if (s == 10)
                arm->val[i] = 7;
            else
                arm->val[i] = (int)rnd(-100, 100);
        }
        top = (int)rnd(-100, 100);
        add_arms(t, arm, top, 1);
        if (s == 9) {
            int who = 2 * m - 1;

            t->val[who] = t->val[who] == 100 ? 99 : t->val[who] + 1;
        } else if (s == 10) {
            drop_leaf(t, 2 * m);
        } else if (s == 11) {
            tree_add(t, 2 * m, 0, arm->val[m - 1]);
        }
    }
    put_tree(t);
    tree_free(t);
    tree_free(arm);
}

void gen(int small)
{
    if (small)
        gen_small();
    else
        gen_full((int)((gen_seed() - 1) % 14));
}
