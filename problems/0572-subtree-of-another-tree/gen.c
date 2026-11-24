/* root and subRoot. Small: a tiny tree, with subRoot one of its branches, that
 * branch changed, cut short or reflected, or another tree. At full size:
 * seed 1 is a left path of 2000 equal values against a path of 1000 whose last
 * value differs (the slowest for comparing at every node), 2 right paths of
 * equal values (2000, 1000; true), 3 a zigzag and its lower 1000 nodes (true),
 * 4 a random shape and its largest branch of at most 1000 nodes (true), 5 a
 * complete tree of equal values and such a branch with one leaf changed, 6 the
 * same with the branch reflected, 7 a path whose deepest value differs against
 * a path of the rest (contained, not equal), 8 a random tree of 1000 nodes and
 * itself (true), 9 a random tree of 999 and itself with one leaf more, and 10 a
 * complete tree with one inner node -10^4 against that value alone. Seeds 11
 * and 12 are caterpillars down the left and the right, against their branch of
 * 1000 nodes with its last leaf changed. */

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

/* Copies node v and everything below it into the empty tree out, breadth
 * first, so that out's last node is a leaf. */
static void copy_branch(const gen_tree *t, int v, gen_tree *out)
{
    int *src = malloc((size_t)(t->n + 1) * sizeof *src);
    int *dst = malloc((size_t)(t->n + 1) * sizeof *dst);
    int head = 0, tail = 1;

    src[0] = v;
    dst[0] = tree_add(out, -1, 0, t->val[v]);
    while (head < tail) {
        int a = src[head], b = dst[head], k;

        head++;
        for (k = 0; k < 2; k++) {
            int c = k ? t->right[a] : t->left[a];

            if (c >= 0) {
                int made = tree_add(out, b, k, t->val[c]);

                src[tail] = c;
                dst[tail] = made;
                tail++;
            }
        }
    }
    free(src);
    free(dst);
}

/* The node with the largest branch of at most limit nodes. Children have
 * higher numbers than their parents in every shape used here. */
static int largest_branch(const gen_tree *t, int limit)
{
    int *size = malloc((size_t)(t->n + 1) * sizeof *size);
    int i, best = t->n - 1;

    for (i = t->n - 1; i >= 0; i--) {
        int l = t->left[i], r = t->right[i];

        size[i] = 1 + (l >= 0 ? size[l] : 0) + (r >= 0 ? size[r] : 0);
        if (size[i] <= limit && size[i] > size[best])
            best = i;
    }
    free(size);
    return best;
}

static void reflect_all(gen_tree *t)
{
    int i;

    for (i = 0; i < t->n; i++) {
        int c = t->left[i];

        t->left[i] = t->right[i];
        t->right[i] = c;
    }
}

/* Unlinks node x, a leaf that isn't the root, from its parent. */
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
    gen_tree *t = tree_new(), *sub = tree_new();
    int n = (int)rnd(1, 7), shape = (int)rnd(0, 4), kind = (int)rnd(0, 4), i;

    make_shape(t, n, shape);
    for (i = 0; i < n; i++)
        t->val[i] = (int)rnd(-1, 2);
    if (kind == 4) {
        int m = (int)rnd(1, 4), other = (int)rnd(0, 4);

        make_shape(sub, m, other);
        for (i = 0; i < m; i++)
            sub->val[i] = (int)rnd(-1, 2);
    } else {
        int v = (int)rnd(0, n - 1);

        copy_branch(t, v, sub);
        if (kind == 1) {
            int who = (int)rnd(0, sub->n - 1);

            sub->val[who] = sub->val[who] == 2 ? 1 : sub->val[who] + 1;
        } else if (kind == 2 && sub->n > 1) {
            drop_leaf(sub, sub->n - 1);
        } else if (kind == 3) {
            reflect_all(sub);
        }
    }
    put_tree(t);
    put_tree(sub);
    tree_free(t);
    tree_free(sub);
}

static void gen_full(int s)
{
    gen_tree *t = tree_new(), *sub = tree_new();
    int n = s == 7 ? 1000 : s == 8 ? 999 : 2000, i;

    if (s == 0 || s == 6) {
        tree_chain(t, n, TREE_LEFT);
        tree_chain(sub, 1000, TREE_LEFT);
        for (i = 0; i < n; i++)
            t->val[i] = s == 0 ? 7 : 5;
        for (i = 0; i < 1000; i++)
            sub->val[i] = s == 0 ? 7 : 5;
        if (s == 0)
            sub->val[999] = -7;
        else
            t->val[n - 1] = 6;
    } else if (s == 1) {
        tree_chain(t, n, TREE_RIGHT);
        tree_chain(sub, 1000, TREE_RIGHT);
        for (i = 0; i < n; i++)
            t->val[i] = -10000;
        for (i = 0; i < 1000; i++)
            sub->val[i] = -10000;
    } else if (s == 2) {
        tree_chain(t, n, TREE_ZIGZAG);
        for (i = 0; i < n; i++)
            t->val[i] = (int)rnd(-10000, 10000);
        copy_branch(t, n - 1000, sub);
    } else if (s == 3 || s == 7 || s == 8) {
        tree_random(t, n);
        for (i = 0; i < n; i++)
            t->val[i] = (int)rnd(-10000, 10000);
        copy_branch(t, s == 3 ? largest_branch(t, 1000) : 0, sub);
        if (s == 8)
            tree_add(sub, sub->n - 1, 0, 10000);
    } else if (s == 4 || s == 5) {
        tree_complete(t, n);
        for (i = 0; i < n; i++)
            t->val[i] = s == 4 ? 1 : -10000;
        copy_branch(t, largest_branch(t, 1000), sub);
        if (s == 4)
            sub->val[sub->n - 1] = 2;
        else
            reflect_all(sub);
    } else if (s >= 10) {
        make_caterpillar(t, n, s - 10);
        for (i = 0; i < n; i++)
            t->val[i] = (int)rnd(-10000, 10000);
        copy_branch(t, largest_branch(t, 1000), sub);
        i = sub->n - 1;
        sub->val[i] = sub->val[i] == 10000 ? 9999 : sub->val[i] + 1;
    } else {
        tree_complete(t, n);
        for (i = 0; i < n; i++)
            t->val[i] = 10000;
        t->val[1] = -10000;
        tree_add(sub, -1, 0, -10000);
    }
    put_tree(t);
    put_tree(sub);
    tree_free(t);
    tree_free(sub);
}

void gen(int small)
{
    if (small)
        gen_small();
    else
        gen_full((int)((gen_seed() - 1) % 12));
}
