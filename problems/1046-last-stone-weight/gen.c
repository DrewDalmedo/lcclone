/* Stone weights in no particular order. Small cases have 1 to 8 stones,
 * with weights up to 4 or up to 12 (the seed picks), so ties and stones
 * that cancel out come up often. Full-size cases have 30 stones: weights
 * from the whole range 1 to 1000, 30 different weights (to stretch a
 * heap), or a few weights near 1000 repeated many times. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 30, i, k, t;
    int stones[30], pool[1000];

    if (small) {
        int top = gen_seed() % 2 ? 4 : 12;

        for (i = 0; i < n; i++)
            stones[i] = (int)rnd(1, top);
    } else {
        switch ((int)(gen_seed() % 3)) {
        case 0:
            for (i = 0; i < n; i++)
                stones[i] = (int)rnd(1, 1000);
            break;
        case 1:
            /* The first 30 of a random order of 1..1000. */
            for (i = 0; i < 1000; i++)
                pool[i] = i + 1;
            for (i = 0; i < n; i++) {
                k = (int)rnd(i, 999);
                t = pool[i];
                pool[i] = pool[k];
                pool[k] = t;
                stones[i] = pool[i];
            }
            break;
        default:
            for (i = 0; i < n; i++)
                stones[i] = (int)rnd(995, 1000);
        }
    }
    put_ints(stones, n);
}
