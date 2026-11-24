/* A search tree of distinct values and a band. At full size, 2 * 10^4 nodes
 * in seven shapes: left path, right path, zigzag, complete, random, and
 * caterpillars whose spine runs left or right with a leaf on the other side
 * of each spine node.
 *   1 to 7   each shape holding 80001 to 10^5, the band covering them all:
 *            the largest possible answer, 1800010000
 *   8 to 14  each shape with values spread over 1 to 10^5, the band up to 5001
 *            values around the deepest node
 *   15       zigzag, low = high = a number just beside the deepest node's
 *            value, which no node holds: the answer is 0 */
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

static void small_band(int *low, int *high)
{
    int a, b, kind = (int)rnd(0, 9);

    if (kind == 0) {
        *low = 1;
        *high = 100000;
    } else if (kind <= 2) {
        a = (int)rnd(1, 22);
        *low = *high = a;
    } else {
        a = (int)rnd(1, 22);
        b = (int)rnd(1, 22);
        *low = a < b ? a : b;
        *high = a < b ? b : a;
    }
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int kind = small ? 0 : (int)((gen_seed() - 1) % 15);
    int n = small ? (int)rnd(1, 8) : 20000;
    int shape, i, low, high, deep, pos = 0, a, b;
    int *v = malloc((size_t)n * sizeof *v);
    int *order = malloc((size_t)n * sizeof *order);

    if (small)
        shape = (int)rnd(0, 6);
    else
        shape = kind == 14 ? ZIGZAG : kind % 7;
    grow_shape(t, n, shape);
    tree_inorder(t, order);

    if (small) {
        pick_sorted(v, n, 1, 20);
        small_band(&low, &high);
    } else if (kind < 7) {
        for (i = 0; i < n; i++)
            v[i] = 80001 + i;
        low = 80001;
        high = 100000;
    } else {
        /* Each value in the first four of its own five numbers, so the
         * number just above a value is never in the tree. */
        for (i = 0; i < n; i++)
            v[i] = i * 5 + 1 + (int)rnd(0, 3);
        v[0] = 1;
        v[n - 1] = 100000;
        deep = deepest_node(t);
        for (i = 0; i < n; i++)
            if (order[i] == deep)
                pos = i;
        if (kind < 14) {
            a = pos > 2500 ? pos - 2500 : 0;
            b = pos + 2500 < n ? pos + 2500 : n - 1;
            low = v[a];
            high = v[b];
        } else {
            low = high = v[pos] == 100000 ? v[pos] - 1 : v[pos] + 1;
        }
    }
    for (i = 0; i < n; i++)
        t->val[order[i]] = v[i];
    put_tree(t);
    put_int(low);
    put_int(high);
    tree_free(t);
    free(v);
    free(order);
}
