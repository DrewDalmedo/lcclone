/* A tree of distinct values and two of them, x and y. Small: tiny trees, with a
 * cousin pair when there is one more often than not. At full size, 100 nodes
 * holding 1 to 100: seed 1 is a left path with the two deepest nodes, 2 a right
 * path with the root and the deepest, 3 a zigzag with any two (all false), 4 a
 * random shape with a cousin pair (true), 5 a complete tree with neighbours in
 * the last row under different parents (true), 6 a complete tree with the last
 * two siblings, 7 two long arms with the ends 49 deep (true), 8 the same arms
 * with ends 50 and 49 deep, 9 a random shape with two nodes of different depth,
 * 10 a random shape with the root and another node, and 11 and 12 caterpillars
 * down the left and the right with one extra leaf, which is a cousin of the
 * deepest spine node (true). */

enum { ANY_PAIR, COUSIN_PAIR, UNEVEN_PAIR };

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

/* 100 nodes: a spine down one side with a leaf on the other side of each spine
 * node, and one more node under the last leaf but one. Sets a to the deepest
 * spine node and b to that extra node: same depth, different parents. */
static void make_cousin_comb(gen_tree *t, int side, int *a, int *b)
{
    int cur = tree_add(t, -1, 0, 0), leaf = -1, before = -1, i;

    for (i = 0; i < 49; i++) {
        int next = tree_add(t, cur, side, 0);

        before = leaf;
        leaf = tree_add(t, cur, !side, 0);
        cur = next;
    }
    *a = cur;
    *b = tree_add(t, before, side, 0);
}

/* The depth and parent of each node. Children have higher numbers than their
 * parents in every shape used here. */
static void find_levels(const gen_tree *t, int *dep, int *par)
{
    int i;

    for (i = 0; i < t->n; i++) {
        dep[i] = 0;
        par[i] = -1;
    }
    for (i = 0; i < t->n; i++) {
        int l = t->left[i], r = t->right[i];

        if (l >= 0) {
            dep[l] = dep[i] + 1;
            par[l] = i;
        }
        if (r >= 0) {
            dep[r] = dep[i] + 1;
            par[r] = i;
        }
    }
}

static int pair_fits(const int *dep, const int *par, int i, int j, int want)
{
    if (want == COUSIN_PAIR)
        return dep[i] == dep[j] && par[i] != par[j];
    if (want == UNEVEN_PAIR)
        return dep[i] != dep[j];
    return 1;
}

/* Picks one of the pairs of nodes that fit, each as likely; 0 if there's none. */
static int pick_pair(int n, const int *dep, const int *par, int want, int *a,
                     int *b)
{
    int i, j, count = 0, k;

    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            count += pair_fits(dep, par, i, j, want);
    if (!count)
        return 0;
    k = (int)rnd(0, count - 1);
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (pair_fits(dep, par, i, j, want) && k-- == 0) {
                *a = i;
                *b = j;
                return 1;
            }
    return 0;
}

/* Prints the tree and the values of nodes a and b, in either order. */
static void put_case(const gen_tree *t, int a, int b)
{
    int swap = rnd_chance(50);

    put_tree(t);
    put_int(t->val[swap ? b : a]);
    put_int(t->val[swap ? a : b]);
}

static void gen_small(void)
{
    gen_tree *t = tree_new();
    int n = (int)rnd(2, 8), shape = (int)rnd(0, 4), want = rnd_chance(60);
    int v[9], dep[8], par[8], a = 0, b = 1, i;

    make_shape(t, n, shape);
    for (i = 0; i < 9; i++)
        v[i] = i + 1;
    rnd_shuffle(v, 9);
    for (i = 0; i < n; i++)
        t->val[i] = v[i];
    find_levels(t, dep, par);
    if (!want || !pick_pair(n, dep, par, COUSIN_PAIR, &a, &b))
        pick_pair(n, dep, par, ANY_PAIR, &a, &b);
    put_case(t, a, b);
    tree_free(t);
}

static void gen_full(int s)
{
    gen_tree *t = tree_new();
    int n = 100, v[100], dep[100], par[100], a = 0, b = 1, i;

    if (s == 6 || s == 7) {
        int cur = tree_add(t, -1, 0, 0);

        for (i = 0; i < 50; i++) /* nodes 1 to 50, depths 1 to 50 */
            cur = tree_add(t, cur, 0, 0);
        cur = 0;
        for (i = 0; i < 49; i++) /* nodes 51 to 99, depths 1 to 49 */
            cur = tree_add(t, cur, 1, 0);
    } else if (s == 3 || s == 8 || s == 9) {
        tree_random(t, n);
    } else if (s == 4 || s == 5) {
        tree_complete(t, n);
    } else if (s >= 10) {
        make_cousin_comb(t, s - 10, &a, &b);
    } else {
        make_shape(t, n, s);
    }
    for (i = 0; i < n; i++)
        v[i] = i + 1;
    rnd_shuffle(v, n);
    for (i = 0; i < n; i++)
        t->val[i] = v[i];
    find_levels(t, dep, par);
    if (s == 0) {
        a = n - 1;
        b = n - 2;
    } else if (s == 1) {
        a = 0;
        b = n - 1;
    } else if (s == 2) {
        pick_pair(n, dep, par, ANY_PAIR, &a, &b);
    } else if (s == 3) {
        pick_pair(n, dep, par, COUSIN_PAIR, &a, &b);
    } else if (s == 4) {
        a = 98; /* the right child of 48 */
        b = 99; /* the left child of 49 */
    } else if (s == 5) {
        a = 97; /* both children of 48 */
        b = 98;
    } else if (s == 6) {
        a = 49;
        b = 99;
    } else if (s == 7) {
        a = 50;
        b = 99;
    } else if (s == 8) {
        pick_pair(n, dep, par, UNEVEN_PAIR, &a, &b);
    } else if (s == 9) {
        a = 0;
        b = (int)rnd(1, n - 1);
    }
    put_case(t, a, b);
    tree_free(t);
}

void gen(int small)
{
    if (small)
        gen_small();
    else
        gen_full((int)((gen_seed() - 1) % 12));
}
