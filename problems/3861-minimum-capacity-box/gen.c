/* Small cases have up to 8 boxes whose capacities lie within 5 of each
 * other, and an item from just below to just above that range, so that
 * ties, exact fits and items that fit nowhere come up often. Full-size
 * cases have 100 boxes, with the seed picking the shape: random
 * capacities, capacities falling from 100 to 1 (the answer far to the
 * right), no box big enough, or only a few boxes of capacity 100 big
 * enough. */
void gen(int small)
{
    int capacity[100];
    int n = small ? (int)rnd(1, 8) : 100, item, lo, hi, shape, i, k;

    if (small) {
        lo = (int)rnd(1, 100);
        hi = lo + 4 > 100 ? 100 : lo + 4;
        for (i = 0; i < n; i++)
            capacity[i] = (int)rnd(lo, hi);
        item = (int)rnd(lo - 1, hi + 1);
        if (item < 1)
            item = 1;
        if (item > 100)
            item = 100;
    } else {
        shape = (int)rnd(0, 3);
        if (shape == 0) {
            for (i = 0; i < n; i++)
                capacity[i] = (int)rnd(1, 100);
            item = (int)rnd(1, 100);
        } else if (shape == 1) {
            for (i = 0; i < n; i++)
                capacity[i] = 100 - i;
            item = (int)rnd(1, 100);
        } else if (shape == 2) {
            item = (int)rnd(2, 100);
            for (i = 0; i < n; i++)
                capacity[i] = (int)rnd(1, item - 1);
        } else {
            item = (int)rnd(95, 100);
            for (i = 0; i < n; i++)
                capacity[i] = (int)rnd(1, item - 1);
            for (k = (int)rnd(1, 3); k > 0; k--) {
                i = (int)rnd(0, n - 1);
                capacity[i] = 100;
            }
        }
    }
    put_ints(capacity, n);
    put_int(item);
}
