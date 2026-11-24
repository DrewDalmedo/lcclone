/* A binary tree whose total tilt fits in an int. At full size, 10^4 nodes:
 * seeds 1 to 3 are paths (left, right, zigzag) whose total is exactly
 * 2^31 - 1, the first all positive, the second all negative, the third with
 * 2072 nodes at the value limit and most of the rest 0. Seed 4 is a path of
 * alternating +1000 and -1000. Seeds 5 and 6 are a caterpillar and a path
 * above a complete tree, scaled so that the total comes close to 2^31 - 1.
 * Seed 7 is a complete tree, +1000 left of the root and -1000 right of it,
 * 8 a random shape and 9 a path, both with random values; bushy shapes
 * can't get near the int limit with values this small. Seed 10 is the
 * caterpillar of seed 5 with its spine down the right. */

#define TILT_CAP 2147483647LL

/* The sum of all tilts. Nodes are numbered parents first. */
static long long tilt_sum(const gen_tree *t, long long *sum)
{
    long long total = 0;
    int i;

    for (i = t->n - 1; i >= 0; i--) {
        long long l = t->left[i] >= 0 ? sum[t->left[i]] : 0;
        long long r = t->right[i] >= 0 ? sum[t->right[i]] : 0;

        sum[i] = l + r + t->val[i];
        total += l > r ? l - r : r - l;
    }
    return total;
}

/* On a path whose values all have one sign, node i counts i times in the
 * total. Raises values, deepest first, until it is exactly 2^31 - 1. */
static void top_up_path(gen_tree *t, int sign, long long *sum)
{
    long long room = TILT_CAP - tilt_sum(t, sum);
    int i;

    for (i = t->n - 1; i > 0 && room > 0; i--) {
        int now = sign > 0 ? t->val[i] : -t->val[i];
        long long k = room / i;

        if (k > 1000 - now)
            k = 1000 - now;
        t->val[i] += sign * (int)k;
        room -= k * i;
    }
}

/* Gives every node the largest equal value that fits, then raises the
 * last nodes by 1 for as long as the total stays within the limit. */
static void scale_up(gen_tree *t, long long *sum)
{
    int n = t->n, i, lo = 0, hi = n, v;

    for (i = 0; i < n; i++)
        t->val[i] = 1;
    v = (int)(TILT_CAP / tilt_sum(t, sum));
    if (v >= 1000) {
        v = 1000;
        hi = 0;
    }
    while (hi - lo > 1) {
        int mid = lo + (hi - lo) / 2;

        for (i = 0; i < n; i++)
            t->val[i] = v + (i >= n - mid);
        if (tilt_sum(t, sum) <= TILT_CAP)
            lo = mid;
        else
            hi = mid;
    }
    for (i = 0; i < n; i++)
        t->val[i] = v + (i >= n - lo);
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(0, 7) : 10000, i;
    int shape = small ? (int)rnd(0, 4) : (int)((gen_seed() - 1) % 10);
    long long *sum = malloc(((size_t)n + 1) * sizeof *sum);

    if (small) {
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
            t->val[i] = (int)rnd(-5, 5);
    } else if (shape == 0) {
        tree_chain(t, n, TREE_LEFT);
        for (i = 0; i < n; i++)
            t->val[i] = 42;
        top_up_path(t, 1, sum);
    } else if (shape == 1) {
        tree_chain(t, n, TREE_RIGHT);
        for (i = 0; i < n; i++)
            t->val[i] = -42;
        top_up_path(t, -1, sum);
    } else if (shape == 2) {
        tree_chain(t, n, TREE_ZIGZAG);
        for (i = 0; i < n; i++)
            t->val[i] = i < 2072 ? 1000 : 0;
        top_up_path(t, 1, sum);
    } else if (shape == 3) {
        tree_chain(t, n, TREE_LEFT);
        for (i = 0; i < n; i++)
            t->val[i] = i % 2 ? -1000 : 1000;
    } else if (shape == 4 || shape == 9) {
        int last = tree_add(t, -1, 0, 0), side = shape == 9;

        for (i = 1; i < n / 2; i++) {
            int next = tree_add(t, last, side, 0);

            tree_add(t, last, !side, 0);
            last = next;
        }
        tree_add(t, last, !side, 0);
        scale_up(t, sum);
    } else if (shape == 5) {
        int last = -1, base;

        for (i = 0; i < 7000; i++)
            last = tree_add(t, last, 0, 0);
        base = t->n;
        for (i = 0; i < n - 7000; i++)
            tree_add(t, i ? base + (i - 1) / 2 : last, i ? (i - 1) % 2 : 0, 0);
        scale_up(t, sum);
    } else if (shape == 6) {
        int *side = malloc((size_t)n * sizeof *side);

        tree_complete(t, n);
        side[0] = 1;
        for (i = 1; i < n; i++)
            side[i] = i <= 2 ? (i == 1 ? 1 : -1) : side[(i - 1) / 2];
        for (i = 0; i < n; i++)
            t->val[i] = side[i] * 1000;
        free(side);
    } else {
        if (shape == 7)
            tree_random(t, n);
        else
            tree_chain(t, n, TREE_RIGHT);
        for (i = 0; i < n; i++)
            t->val[i] = (int)rnd(-1000, 1000);
        while (tilt_sum(t, sum) > TILT_CAP)
            for (i = 0; i < n; i++)
                t->val[i] /= 2;
    }
    put_tree(t);
    free(sum);
    tree_free(t);
}
