/* A binary tree and a target, values and target within [-1000, 1000]. At
 * full size, 5000 nodes:
 *   seed 1  left path climbing to 2.5 * 10^6 and back down onto the target;
 *   seed 2  right path whose sum is the target plus 76 * 65536 (false, but
 *           true for a 16-bit total);
 *   seed 3  zigzag, a random walk steered to end on the target;
 *   seed 4  complete tree where only the last leaf matches;
 *   seed 5  a path leading into a caterpillar, only the deepest leaf matches;
 *   seed 6  the same on the other side, every leaf one above the target;
 *   seed 7  a random shape with random values;
 *   seed 8  complete tree where only leaf 4901 matches (a circular queue of
 *           2048 entries loses it);
 *   seed 9  caterpillar of 2500 levels down the left, only the deepest leaf
 *           matches;
 *   seed 10 the same down the right, every leaf one above the target. */

/* A path of c nodes down one side, then s spine nodes that each carry a leaf
 * on the other side. The path's values cancel and the spine's are 0, so a
 * leaf's route sums to the leaf's own value. Returns the deepest leaf. */
static int broom_shape(gen_tree *t, int c, int s, int side, int leaf_val)
{
    int last = -1, leaf = -1, i;

    for (i = 0; i < c + s; i++) {
        last = tree_add(t, last, side, i < c ? (i % 2 ? -1000 : 1000) : 0);
        if (i >= c)
            leaf = tree_add(t, last, !side, leaf_val);
    }
    return leaf;
}

static void small_case(gen_tree *t)
{
    int n = (int)rnd(0, 7), shape = (int)rnd(0, 4), i, target;

    if (shape == 0)
        tree_chain(t, n, TREE_LEFT);
    else if (shape == 1)
        tree_chain(t, n, TREE_RIGHT);
    else if (shape == 2)
        tree_chain(t, n, TREE_ANY);
    else if (shape == 3)
        tree_complete(t, n);
    else
        tree_random(t, n);
    for (i = 0; i < n; i++)
        t->val[i] = (int)rnd(-3, 3);
    target = (int)rnd(-4, 4);
    if (n > 0 && rnd_chance(50)) {
        /* The score of one random fall, so that true comes up often. */
        int p = t->root, sum = t->val[p];

        while (t->left[p] >= 0 || t->right[p] >= 0) {
            int go_right = t->left[p] < 0 || (t->right[p] >= 0 && rnd_chance(50));

            p = go_right ? t->right[p] : t->left[p];
            sum += t->val[p];
        }
        target = sum;
    }
    put_tree(t);
    put_int(target);
}

static void full_case(gen_tree *t)
{
    int n = 5000, shape = (int)((gen_seed() - 1) % 10), i, target, sum = 0;

    if (shape == 0) {
        tree_chain(t, n, TREE_LEFT);
        target = (int)rnd(0, 1000);
        for (i = 0; i < n - 1; i++)
            t->val[i] = i < n / 2 ? 1000 : -1000;
        t->val[n - 1] = target - 1000;
    } else if (shape == 1) {
        int cut;

        tree_chain(t, n, TREE_RIGHT);
        target = (int)rnd(-1000, 1000);
        for (i = 0; i < n; i++)
            t->val[i] = 1000;
        cut = 5000000 - (76 * 65536 + target);
        for (i = n - 1; cut > 0; i--) {
            int d = cut < 2000 ? cut : 2000;

            t->val[i] -= d;
            cut -= d;
        }
    } else if (shape == 2) {
        tree_chain(t, n, TREE_ZIGZAG);
        target = (int)rnd(-1000, 1000);
        for (i = 0; i < n; i++) {
            int reach = 1000 * (n - 1 - i);
            int lo = target - sum - reach, hi = target - sum + reach;

            if (lo < -1000)
                lo = -1000;
            if (hi > 1000)
                hi = 1000;
            t->val[i] = (int)rnd(lo, hi);
            sum += t->val[i];
        }
    } else if (shape == 3) {
        tree_complete(t, n);
        target = (int)rnd(-1000, 999);
        for (i = 0; i < n; i++)
            t->val[i] = 1000;
        for (i = n - 1; i > 0; i = (i - 1) / 2)
            t->val[i] = 0;
        t->val[0] = 0;
        t->val[n - 1] = target;
    } else if (shape == 4) {
        int deepest;

        target = (int)rnd(-1000, 999);
        deepest = broom_shape(t, 1666, 1667, 0, 1000);
        t->val[deepest] = target;
    } else if (shape == 5) {
        target = (int)rnd(-1000, 999);
        broom_shape(t, 1666, 1667, 1, target + 1);
    } else if (shape == 7) {
        tree_complete(t, n);
        target = (int)rnd(-1000, 999);
        for (i = 0; i < n; i++)
            t->val[i] = 1000;
        for (i = 4901; i > 0; i = (i - 1) / 2)
            t->val[i] = 0;
        t->val[0] = 0;
        t->val[4901] = target;
    } else if (shape == 8) {
        int deepest;

        target = (int)rnd(-1000, 999);
        deepest = broom_shape(t, 0, 2500, 0, target + 1);
        t->val[deepest] = target;
    } else if (shape == 9) {
        target = (int)rnd(-1000, 999);
        broom_shape(t, 0, 2500, 1, target + 1);
    } else {
        tree_random(t, n);
        for (i = 0; i < n; i++)
            t->val[i] = (int)rnd(-1000, 1000);
        target = (int)rnd(-1000, 1000);
    }
    put_tree(t);
    put_int(target);
}

void gen(int small)
{
    gen_tree *t = tree_new();

    if (small)
        small_case(t);
    else
        full_case(t);
    tree_free(t);
}
