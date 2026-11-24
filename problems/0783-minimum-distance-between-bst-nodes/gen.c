/* Distinct positions from 0 to 10^5, laid over a shape in order. At full
 * size, 100 nodes:
 *   1  left path, stations exactly 1010 apart, the largest answer at n = 100
 *   2  right path, the closest pair (99999 and 10^5) at the deep end
 *   3  zigzag, the closest pair is the deepest node and its neighbour
 *   4  random shape, random positions
 *   5  complete tree, the closest pair is the root and its neighbour below
 *   6  path turning at random, the closest pair is 0 and 1 */
static void draw_positions(int *v, int n, int top)
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

static int place_of(const int *order, int n, int node)
{
    int i;

    for (i = 0; i < n; i++)
        if (order[i] == node)
            return i;
    return 0;
}

static void full_positions(const gen_tree *t, const int *order, int *v, int n,
                           int shape)
{
    int i, at;

    switch (shape) {
    case 0:
        for (i = 0; i < n; i++)
            v[i] = 1010 * i;
        break;
    case 1:
        for (i = 0; i < n; i++)
            v[i] = 1000 * i;
        v[n - 2] = 99999;
        v[n - 1] = 100000;
        break;
    case 2:
        for (i = 0; i < n; i++)
            v[i] = 1000 * i + 5;
        at = place_of(order, n, n - 1);
        if (at == 0)
            at = 1;
        v[at] = v[at - 1] + 3;
        break;
    case 3:
        draw_positions(v, n, 100000);
        break;
    case 4:
        for (i = 0; i < n; i++)
            v[i] = 1000 * i;
        at = place_of(order, n, t->root);
        v[at - 1] = v[at] - 2;
        break;
    default:
        for (i = 0; i < n; i++)
            v[i] = 1000 * i;
        v[1] = 1;
        break;
    }
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(2, 8) : 100;
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
        int top = pick == 0 ? n + 3 : pick == 1 ? 25 : pick == 2 ? 500 : 100000;

        draw_positions(v, n, top);
        if (pick == 3 && rnd_chance(50)) {
            v[0] = 0;
            v[n - 1] = 100000;
        }
    } else {
        full_positions(t, order, v, n, shape);
    }
    for (i = 0; i < n; i++)
        t->val[order[i]] = v[i];
    put_tree(t);
    tree_free(t);
    free(v);
    free(order);
}
