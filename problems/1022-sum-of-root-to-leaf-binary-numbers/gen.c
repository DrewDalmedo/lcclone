/* A binary tree of bits whose leaves' numbers sum to at most 2^31 - 1. At
 * full size, 1000 nodes:
 *   seed 1  left path: 969 zeros, then 31 ones (exactly 2^31 - 1);
 *   seed 2  right path: 999 zeros, then a one (a place value would shift by
 *           up to 999);
 *   seed 3  zigzag: 969 zeros, then a 1 and 30 random bits;
 *   seed 4  938 zeros, then two arms of 31 bits whose numbers add up to
 *           exactly 2^31 - 1;
 *   seed 5  caterpillar: a spine of 500 zeros with 500 leaves of 1;
 *   seed 6  complete tree, random bits;
 *   seed 7  a random shape, with zeros above its lowest 20 levels so that the
 *           sum fits;
 *   seed 8  caterpillar down the left: 500 zeros with 500 right leaves of 1. */

/* Writes the low `len` bits of v onto nodes first, first + step, ... */
static void write_bits(gen_tree *t, int first, int step, int len, long long v)
{
    int i;

    for (i = 0; i < len; i++)
        t->val[first + i * step] = (int)((v >> (len - 1 - i)) & 1);
}

void gen(int small)
{
    gen_tree *t = tree_new();
    int i;

    if (small) {
        int n = (int)rnd(1, 8), shape = (int)rnd(0, 4);

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
            t->val[i] = (int)rnd(0, 1);
    } else {
        int n = 1000, shape = (int)((gen_seed() - 1) % 8);

        if (shape == 0) {
            tree_chain(t, n, TREE_LEFT);
            write_bits(t, n - 31, 1, 31, 2147483647LL);
        } else if (shape == 1) {
            tree_chain(t, n, TREE_RIGHT);
            t->val[n - 1] = 1;
        } else if (shape == 2) {
            long long low = rnd(0, (1LL << 30) - 1);

            tree_chain(t, n, TREE_ZIGZAG);
            write_bits(t, n - 31, 1, 31, (1LL << 30) + low);
        } else if (shape == 3) {
            long long a = rnd(1, 2147483646LL);
            int stem = -1, l, r;

            for (i = 0; i < 938; i++)
                stem = tree_add(t, stem, (int)(i % 2), 0);
            l = stem;
            r = stem;
            for (i = 0; i < 31; i++) {
                l = tree_add(t, l, 0, 0);
                r = tree_add(t, r, 1, 0);
            }
            write_bits(t, 938, 2, 31, a);
            write_bits(t, 939, 2, 31, 2147483647LL - a);
        } else if (shape == 4) {
            int last = -1;

            for (i = 0; i < n / 2; i++) {
                last = tree_add(t, last, 1, 0);
                tree_add(t, last, 0, 1);
            }
        } else if (shape == 5) {
            tree_complete(t, n);
            for (i = 0; i < n; i++)
                t->val[i] = (int)rnd(0, 1);
        } else if (shape == 7) {
            int last = -1;

            for (i = 0; i < n / 2; i++) {
                last = tree_add(t, last, 0, 0);
                tree_add(t, last, 1, 1);
            }
        } else {
            int *level = malloc((size_t)n * sizeof *level), deepest;

            tree_random(t, n);
            deepest = tree_depth(t);
            level[t->root] = 1;
            for (i = 0; i < n; i++) {
                if (t->left[i] >= 0)
                    level[t->left[i]] = level[i] + 1;
                if (t->right[i] >= 0)
                    level[t->right[i]] = level[i] + 1;
                t->val[i] = level[i] > deepest - 20 ? (int)rnd(0, 1) : 0;
            }
            free(level);
        }
    }
    put_tree(t);
    tree_free(t);
}
