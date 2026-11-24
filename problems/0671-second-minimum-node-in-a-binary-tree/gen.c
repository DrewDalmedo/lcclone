/* A tree where each node has 0 or 2 children and holds the smaller of its
 * children's values. The leaves are drawn, and the inner nodes follow. At
 * full size, 25 nodes:
 *   seed  shape                   leaves
 *   1     caterpillar, spine left   all 1 but the deepest, 2^31 - 1
 *   2     caterpillar, spine right  all 1 but the deepest, 2^31 - 1
 *   3     caterpillar, zigzag       all 2^31 - 2 but the deepest, 2^31 - 1
 *   4     complete                  all 2^31 - 1 (no second value)
 *   5     random                    mixed, up to 2^31 - 1
 *   6     caterpillar, spine left   mixed
 *   7     caterpillar, spine right  mixed
 *   8     caterpillar, zigzag       all 1 (no second value)
 *   9     complete                  mixed
 *   10    random                    all 2^31 - 2 but the last, 2^31 - 1 */
enum { CAT_LEFT, CAT_RIGHT, CAT_ZIGZAG, FULL_COMPLETE, FULL_RANDOM };
enum { ONE_HIGH, TOP_PAIR, ALL_TOP, ALL_ONE, MIXED };

#define TOP 2147483647

static void grow_spine(gen_tree *t, int n, int how)
{
    int at = tree_add(t, -1, 0, 0), step = 0, side, next;

    while (t->n < n) {
        side = how == CAT_ZIGZAG ? step % 2 : how == CAT_RIGHT;
        step++;
        next = tree_add(t, at, side, 0);
        tree_add(t, at, !side, 0);
        at = next;
    }
}

/* Splits a random leaf until there are n nodes. */
static void grow_splits(gen_tree *t, int n)
{
    int leaves[16], count = 1, at, a, b;

    leaves[0] = tree_add(t, -1, 0, 0);
    while (t->n < n) {
        at = (int)rnd(0, count - 1);
        a = tree_add(t, leaves[at], 0, 0);
        b = tree_add(t, leaves[at], 1, 0);
        leaves[at] = a;
        leaves[count++] = b;
    }
}

static void grow_shape(gen_tree *t, int n, int shape)
{
    if (shape == FULL_COMPLETE)
        tree_complete(t, n);
    else if (shape == FULL_RANDOM)
        grow_splits(t, n);
    else
        grow_spine(t, n, shape);
}

/* Children are numbered after their parents, so going backwards sets each
 * node after both of its children. */
static void settle_matches(gen_tree *t)
{
    int i, a, b;

    for (i = t->n - 1; i >= 0; i--) {
        a = t->left[i];
        b = t->right[i];
        if (a >= 0)
            t->val[i] = t->val[a] < t->val[b] ? t->val[a] : t->val[b];
    }
}

static void score_leaves(gen_tree *t, int regime)
{
    int i, last = t->n - 1, base = (int)rnd(1, 1000), x;

    for (i = 0; i <= last; i++) {
        if (t->left[i] >= 0)
            continue;
        if (regime == ONE_HIGH)
            x = i == last ? TOP : 1;
        else if (regime == TOP_PAIR)
            x = i == last ? TOP : TOP - 1;
        else if (regime == ALL_TOP)
            x = TOP;
        else if (regime == ALL_ONE)
            x = 1;
        else if (rnd_chance(35))
            x = base;
        else
            x = (int)rnd((long long)base + 1, TOP);
        t->val[i] = x;
    }
}

static void score_small(gen_tree *t)
{
    int kind = (int)rnd(0, 9), base = (int)rnd(1, 3), i, x;

    for (i = 0; i < t->n; i++) {
        if (t->left[i] >= 0)
            continue;
        if (kind <= 1)
            x = base;
        else if (kind <= 3)
            x = rnd_chance(70) ? base : TOP;
        else if (kind == 4)
            x = rnd_chance(50) ? TOP - 1 : TOP;
        else
            x = base + (int)rnd(0, 3);
        t->val[i] = x;
    }
}

void gen(int small)
{
    static const int shapes[10] = {
        CAT_LEFT, CAT_RIGHT, CAT_ZIGZAG, FULL_COMPLETE, FULL_RANDOM,
        CAT_LEFT, CAT_RIGHT, CAT_ZIGZAG, FULL_COMPLETE, FULL_RANDOM,
    };
    static const int regimes[10] = {
        ONE_HIGH, ONE_HIGH, TOP_PAIR, ALL_TOP, MIXED,
        MIXED, MIXED, ALL_ONE, MIXED, TOP_PAIR,
    };
    gen_tree *t = tree_new();
    int index = small ? 0 : (int)((gen_seed() - 1) % 10);
    int n = small ? 2 * (int)rnd(0, 5) + 1 : 25;
    int shape = small ? (int)rnd(0, 4) : shapes[index];

    grow_shape(t, n, shape);
    if (small)
        score_small(t);
    else
        score_leaves(t, regimes[index]);
    settle_matches(t);
    put_tree(t);
    tree_free(t);
}
