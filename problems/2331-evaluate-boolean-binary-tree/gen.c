/* A full binary tree: leaves hold 0 or 1, inner nodes 2 (OR) or 3 (AND). At
 * full size, 999 nodes (a full tree's count is odd), and the caterpillars
 * have 500 levels:
 *   seed 1  caterpillar down the left, all OR, only the deepest leaf true;
 *   seed 2  down the right, all AND, only the deepest leaf false;
 *   seed 3  down the right, all OR, every leaf false;
 *   seed 4  down the left, all AND, every leaf true;
 *   seed 5  zigzag, mixed gates, each side leaf neutral for its gate, so the
 *           deepest leaf (true) decides;
 *   seed 6  the same from the other side, the deepest leaf false;
 *   seed 7  complete tree, random gates and leaves;
 *   seed 8  a random full tree, random gates and leaves. */

/* `gates` inner nodes in a line, each with a leaf on the side away from the
 * line. way: 0 line goes left, 1 right, 2 and 3 zigzag from either side.
 * kind: 2 all OR, 3 all AND, 0 random. A side leaf gets `side_leaf`, or the
 * value that leaves its gate's other input unchanged when side_leaf is -1;
 * the deepest leaf gets `bottom`. */
static void caterpillar_gates(gen_tree *t, int gates, int way, int kind,
                              int side_leaf, int bottom)
{
    int cur = tree_add(t, -1, 0, 0), i;

    for (i = 0; i < gates; i++) {
        int dir = way < 2 ? way : (i + way) % 2;
        int op = kind ? kind : (int)rnd(2, 3);
        int leaf = side_leaf >= 0 ? side_leaf : op == 3;
        int next;

        t->val[cur] = op;
        tree_add(t, cur, !dir, leaf);
        next = tree_add(t, cur, dir, 0);
        cur = next;
    }
    t->val[cur] = bottom;
}

/* Grows a full tree: a random leaf gets two children, `gates` times. */
static void grow_full(gen_tree *t, int gates)
{
    int *tips = malloc(((size_t)gates + 1) * sizeof *tips);
    int ntips = 1, i;

    tips[0] = tree_add(t, -1, 0, 0);
    for (i = 0; i < gates; i++) {
        int k = (int)rnd(0, ntips - 1), p = tips[k];

        tips[k] = tree_add(t, p, 0, 0);
        tips[ntips++] = tree_add(t, p, 1, 0);
    }
    free(tips);
}

/* Random gates on the inner nodes and random bits on the leaves. */
static void fill_circuit(gen_tree *t)
{
    int i;

    for (i = 0; i < t->n; i++)
        t->val[i] = t->left[i] >= 0 ? (int)rnd(2, 3) : (int)rnd(0, 1);
}

void gen(int small)
{
    gen_tree *t = tree_new();

    if (small) {
        grow_full(t, (int)rnd(0, 4));
        fill_circuit(t);
    } else {
        int gates = 499;

        switch ((gen_seed() - 1) % 8) {
        case 0:
            caterpillar_gates(t, gates, 0, 2, 0, 1);
            break;
        case 1:
            caterpillar_gates(t, gates, 1, 3, 1, 0);
            break;
        case 2:
            caterpillar_gates(t, gates, 1, 2, 0, 0);
            break;
        case 3:
            caterpillar_gates(t, gates, 0, 3, 1, 1);
            break;
        case 4:
            caterpillar_gates(t, gates, 2, 0, -1, 1);
            break;
        case 5:
            caterpillar_gates(t, gates, 3, 0, -1, 0);
            break;
        case 6:
            tree_complete(t, 2 * gates + 1);
            fill_circuit(t);
            break;
        default:
            grow_full(t, gates);
            fill_circuit(t);
            break;
        }
    }
    put_tree(t);
    tree_free(t);
}
