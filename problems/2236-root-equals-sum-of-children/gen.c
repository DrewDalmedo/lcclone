/* Three nodes: the root, then its left and right child. At full size the
 * values sit at their limits:
 *   seed 1  100 = 100 + 0 (true);
 *   seed 2  100 as a split of two items in [0, 100] (true);
 *   seed 3  -100 as a split of two items in [-100, 0] (true);
 *   seed 4  100, 100, 100: the items sum past any total (false);
 *   seed 5  -100, -100, -100 (false);
 *   seed 6  -56, 100, 100: an 8-bit sum would wrap onto the total (false);
 *   seed 7  0 = -100 + 100 (true);
 *   seed 8  100 against 100 + -1, off by one at the limit (false). */
void gen(int small)
{
    gen_tree *t = tree_new();
    int total, a, b;

    tree_complete(t, 3);
    if (small) {
        a = (int)rnd(-4, 4);
        b = (int)rnd(-4, 4);
        total = rnd_chance(50) ? a + b : (int)rnd(-8, 8);
    } else {
        switch ((gen_seed() - 1) % 8) {
        case 0:
            total = 100, a = 100, b = 0;
            break;
        case 1:
            total = 100;
            a = (int)rnd(0, 100);
            b = 100 - a;
            break;
        case 2:
            total = -100;
            a = (int)rnd(-100, 0);
            b = -100 - a;
            break;
        case 3:
            total = 100, a = 100, b = 100;
            break;
        case 4:
            total = -100, a = -100, b = -100;
            break;
        case 5:
            total = -56, a = 100, b = 100;
            break;
        case 6:
            total = 0, a = -100, b = 100;
            break;
        default:
            total = 100, a = 100, b = -1;
            break;
        }
    }
    t->val[0] = total;
    t->val[1] = a;
    t->val[2] = b;
    put_tree(t);
    tree_free(t);
}
