/* A search tree of distinct values from 0 to 10^5, laid over a shape in
 * order. At full size, 10^4 nodes:
 *   1  left path, the closest pair (0 and 1) at the deep end
 *   2  right path, the closest pair (99999 and 10^5) at the deep end
 *   3  zigzag, every neighbour 10 apart, the largest answer 10^4 nodes allow
 *   4  random shape, a random set of values
 *   5  complete tree, the closest pair the root and its in-order neighbour,
 *      many levels below it
 *   6  path turning at random, one pair 7 apart somewhere inside */
static void draw_sorted(int *v, int n, int top)
{
    int i = 0, j, x, dup;

    while (i < n) {
        x = (int)rnd(0, top);
        dup = 0;
        for (j = 0; j < i; j++)
            if (v[j] == x)
                dup = 1;
        if (dup)
            continue;
        for (j = i; j > 0 && v[j - 1] > x; j--)
            v[j] = v[j - 1];
        v[j] = x;
        i++;
    }
}

static void full_values(const gen_tree *t, const int *order, int *v, int n,
                        int shape)
{
    int i, x, need, at;
    long long r;

    switch (shape) {
    case 0:
        for (i = 0; i < n; i++)
            v[i] = 10 * i;
        v[1] = 1;
        break;
    case 1:
        for (i = 0; i < n; i++)
            v[i] = 10 * i;
        v[n - 2] = 99999;
        v[n - 1] = 100000;
        break;
    case 2:
        for (i = 0; i < n; i++)
            v[i] = 10 * (i + 1);
        break;
    case 3:
        need = n;
        for (x = 0; x <= 100000 && need > 0; x++) {
            r = rnd(1, 100001 - x);
            if (r <= need) {
                v[n - need] = x;
                need--;
            }
        }
        break;
    case 4:
        for (i = 0; i < n; i++)
            v[i] = 10 * i;
        at = 1;
        for (i = 1; i < n; i++)
            if (order[i] == t->root)
                at = i;
        v[at - 1] = v[at] - 3;
        break;
    default:
        for (i = 0; i < n; i++)
            v[i] = 10 * i + 5;
        at = (int)rnd(1, n - 1);
        v[at] = v[at - 1] + 7;
        break;
    }
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(2, 8) : 10000;
    int shape = small ? (int)rnd(0, 5) : (int)((gen_seed() - 1) % 6);
    int *v = malloc((size_t)n * sizeof *v);
    int *order = malloc((size_t)n * sizeof *order);
    int i;

    if (shape == 0)
        tree_chain(t, n, TREE_LEFT);
    else if (shape == 1)
        tree_chain(t, n, TREE_RIGHT);
    else if (shape == 2)
        tree_chain(t, n, small ? TREE_ANY : TREE_ZIGZAG);
    else if (shape == 3 || small)
        tree_random(t, n);
    else if (shape == 4)
        tree_complete(t, n);
    else
        tree_chain(t, n, TREE_ANY);
    tree_inorder(t, order);
    if (small) {
        int pick = (int)rnd(0, 3);
        int top = pick == 0 ? n + 3 : pick == 1 ? 20 : pick == 2 ? 300 : 100000;

        draw_sorted(v, n, top);
        if (pick == 3 && rnd_chance(50)) {
            v[0] = 0;
            v[n - 1] = 100000;
        }
    } else {
        full_values(t, order, v, n, shape);
    }
    for (i = 0; i < n; i++)
        t->val[order[i]] = v[i];
    put_tree(t);
    tree_free(t);
    free(v);
    free(order);
}
