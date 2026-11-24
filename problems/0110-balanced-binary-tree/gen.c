/* A binary tree, often a balanced one. At full size, 5000 nodes: seed 1 is
 * a path of left children, 2 of right children, 3 a zigzag (all false), 4 a
 * complete tree (true), 5 a balanced tree of the greatest height, 17, whose
 * leaves are many levels apart (true), 6 a complete tree where one node
 * deep down fails and all the others pass (false), 7 two bare paths under
 * the root (the root passes, its children don't: false), 8 a random
 * balanced tree (true), 9 a random shape, and 10 and 11 caterpillars with
 * a spine of 2500 nodes down the left and the right (false). */

static int avl_least[20];

static int avl_most(int h)
{
    return (1 << h) - 1;
}

/* Adds a balanced tree of height h and k nodes. With lean set, each node
 * keeps its right branch as small as it can. */
static void avl_grow(gen_tree *t, int parent, int side, int h, int k, int lean)
{
    int id = tree_add(t, parent, side, 0), rest = k - 1, opts[3][4];
    int nopt = 0, o, c, kl;

    if (h == 1)
        return;
    for (o = 0; o < 3; o++) {
        int hl = o == 1 ? h - 2 : h - 1, hr = o == 0 ? h - 2 : h - 1;
        int lo = rest - avl_most(hr), hi = rest - avl_least[hr];

        if (lo < avl_least[hl])
            lo = avl_least[hl];
        if (hi > avl_most(hl))
            hi = avl_most(hl);
        if (lo <= hi) {
            opts[nopt][0] = hl;
            opts[nopt][1] = hr;
            opts[nopt][2] = lo;
            opts[nopt][3] = hi;
            nopt++;
        }
    }
    c = lean ? 0 : (int)rnd(0, nopt - 1);
    kl = lean ? opts[c][3] : (int)rnd(opts[c][2], opts[c][3]);
    if (kl > 0)
        avl_grow(t, id, 0, opts[c][0], kl, lean);
    if (rest - kl > 0)
        avl_grow(t, id, 1, opts[c][1], rest - kl, lean);
}

/* A spine of n / 2 nodes going down one side (side 0 is left), each spine
 * node with a leaf on the other side. An explicit stack that pushes both
 * children holds a leaf per level on one of the two sides. */
static void caterpillar_shape(gen_tree *t, int n, int side)
{
    int last = tree_add(t, -1, 0, 0), i;

    for (i = 1; i < n / 2; i++) {
        int next = tree_add(t, last, side, 0);

        tree_add(t, last, !side, 0);
        last = next;
    }
    tree_add(t, last, !side, 0);
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int n = small ? (int)rnd(0, 7) : 5000, i;
    int shape = small ? (int)rnd(0, 5) : (int)((gen_seed() - 1) % 11);

    avl_least[0] = 0;
    avl_least[1] = 1;
    for (i = 2; i < 20; i++)
        avl_least[i] = avl_least[i - 1] + avl_least[i - 2] + 1;
    if (small) {
        if (shape == 0) {
            tree_chain(t, n, TREE_ANY);
        } else if (shape == 1) {
            tree_complete(t, n);
        } else if (shape == 2) {
            tree_random(t, n);
        } else if (n > 0) {
            int lo = 1, hi;

            while (avl_most(lo) < n)
                lo++;
            hi = lo;
            while (avl_least[hi + 1] <= n)
                hi++;
            avl_grow(t, -1, 0, (int)rnd(lo, hi), n, 0);
        }
    } else if (shape == 0) {
        tree_chain(t, n, TREE_LEFT);
    } else if (shape == 1) {
        tree_chain(t, n, TREE_RIGHT);
    } else if (shape == 2) {
        tree_chain(t, n, TREE_ZIGZAG);
    } else if (shape == 3) {
        tree_complete(t, n);
    } else if (shape == 4) {
        avl_grow(t, -1, 0, 17, n, 1);
    } else if (shape == 5) {
        /* Positions of a complete tree of n + 4. Without these four, the
         * last node of the tenth level keeps its height of 3 through a bare
         * path on its left, and is the only node to fail. */
        int *id = malloc(((size_t)n + 4) * sizeof *id);

        for (i = 0; i < n + 4; i++) {
            if (i == 2046 || (i >= 4092 && i <= 4094))
                continue;
            id[i] = tree_add(t, i ? id[(i - 1) / 2] : -1,
                             i ? (i - 1) % 2 : 0, 0);
        }
        free(id);
    } else if (shape == 6) {
        int l, r;

        tree_add(t, -1, 0, 0);
        l = tree_add(t, 0, 0, 0);
        r = tree_add(t, 0, 1, 0);
        for (i = 3; i < n; i++) {
            if (i % 2)
                l = tree_add(t, l, 0, 0);
            else
                r = tree_add(t, r, 1, 0);
        }
    } else if (shape == 7) {
        avl_grow(t, -1, 0, (int)rnd(13, 17), n, 0);
    } else if (shape == 8) {
        tree_random(t, n);
    } else {
        caterpillar_shape(t, n, shape - 9);
    }
    for (i = 0; i < n; i++)
        t->val[i] = (int)(small ? rnd(-3, 3) : rnd(-10000, 10000));
    put_tree(t);
    tree_free(t);
}
