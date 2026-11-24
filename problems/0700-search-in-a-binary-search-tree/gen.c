/* A search tree of distinct values and a value to look for. At full size,
 * 5000 nodes from 1 to 10^7, in seven shapes: left path, right path, zigzag,
 * complete, random, and caterpillars whose spine runs left or right with a
 * leaf on the other side of each spine node.
 *   1 to 7   each shape, val at its deepest node
 *   8 to 14  each shape, val just beside the deepest node's value and absent,
 *            so that the walk goes all the way down and finds nothing
 *   15, 16   left path and right caterpillar, val halfway down: a branch of
 *            about half the tree comes back */
enum { LEFT_PATH, RIGHT_PATH, ZIGZAG, COMPLETE, RANDOM, LEFT_CAT, RIGHT_CAT };

static void grow_caterpillar(gen_tree *t, int n, int side)
{
    int at = tree_add(t, -1, 0, 0), next;

    while (t->n < n) {
        next = tree_add(t, at, side, 0);
        if (t->n < n)
            tree_add(t, at, !side, 0);
        at = next;
    }
}

static void grow_shape(gen_tree *t, int n, int shape)
{
    if (shape == LEFT_PATH)
        tree_chain(t, n, TREE_LEFT);
    else if (shape == RIGHT_PATH)
        tree_chain(t, n, TREE_RIGHT);
    else if (shape == ZIGZAG)
        tree_chain(t, n, TREE_ZIGZAG);
    else if (shape == COMPLETE)
        tree_complete(t, n);
    else if (shape == RANDOM)
        tree_random(t, n);
    else
        grow_caterpillar(t, n, shape == RIGHT_CAT);
}

/* Parents are numbered before their children in every shape above. */
static int deepest_node(const gen_tree *t)
{
    int *depth = malloc((size_t)t->n * sizeof *depth);
    int i, best = 0;

    depth[0] = 0;
    for (i = 0; i < t->n; i++) {
        if (t->left[i] >= 0)
            depth[t->left[i]] = depth[i] + 1;
        if (t->right[i] >= 0)
            depth[t->right[i]] = depth[i] + 1;
        if (depth[i] > depth[best])
            best = i;
    }
    free(depth);
    return best;
}

/* n sorted distinct values of [lo, hi], each set as likely. */
static void pick_sorted(int *v, int n, int lo, int hi)
{
    int need = n, x;
    long long r;

    for (x = lo; x <= hi && need > 0; x++) {
        r = rnd(1, (long long)hi - x + 1);
        if (r <= need) {
            v[n - need] = x;
            need--;
        }
    }
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int seed = small ? 0 : (int)((gen_seed() - 1) % 16);
    int n = small ? (int)rnd(1, 8) : 5000;
    int shape, i, val, who, kind;
    int *v = malloc((size_t)n * sizeof *v);
    int *order = malloc((size_t)n * sizeof *order);

    if (small)
        shape = (int)rnd(0, 6);
    else if (seed < 14)
        shape = seed % 7;
    else
        shape = seed == 14 ? LEFT_PATH : RIGHT_CAT;
    grow_shape(t, n, shape);

    if (small) {
        pick_sorted(v, n, 1, 16);
    } else {
        /* Each value in the low half of its own bucket of 2000, so the
         * number just above a value is never in the tree. */
        for (i = 0; i < n; i++)
            v[i] = i * 2000 + (int)rnd(1, 1000);
        v[0] = 1;
        v[n - 1] = 10000000;
    }
    tree_inorder(t, order);
    for (i = 0; i < n; i++)
        t->val[order[i]] = v[i];

    if (small) {
        kind = (int)rnd(0, 9);
        if (kind <= 5) {
            who = (int)rnd(0, n - 1);
            val = t->val[who];
        } else if (kind <= 7) {
            val = (int)rnd(1, 17);
        } else {
            val = kind == 8 ? 1 : 10000000;
        }
    } else if (seed < 7) {
        val = t->val[deepest_node(t)];
    } else if (seed < 14) {
        val = t->val[deepest_node(t)];
        val = val == 10000000 ? val - 1 : val + 1;
    } else {
        /* An odd number is a spine node of a caterpillar. */
        val = t->val[(n / 2) | 1];
    }
    put_tree(t);
    put_int(val);
    tree_free(t);
    free(v);
    free(order);
}
