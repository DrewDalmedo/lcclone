/* Small cases: 2 to 10 values from 1 to 4 (ties are common) and a random
 * k up to half the length. Full-size ones have 100 values up to 1000, in
 * a shape picked by the seed: random; all equal; increasing; decreasing;
 * or values from a narrow range so that ties are frequent. */
void gen(int small)
{
    int nums[100], n, k, shape, i, v, lo;

    n = small ? (int)rnd(2, 10) : 100;
    k = (int)rnd(1, n / 2);
    shape = small ? 0 : (int)rnd(0, 4);
    lo = (int)rnd(1, 990);
    for (i = 0; i < n; i++) {
        if (small) {
            v = (int)rnd(1, 4);
        } else if (shape == 0) {
            v = (int)rnd(1, 1000);
        } else if (shape == 1) {
            v = 1000;
        } else if (shape == 2) {
            v = 1 + 10 * i;
        } else if (shape == 3) {
            v = 1000 - 10 * i;
        } else {
            v = (int)rnd(lo, lo + 10);
        }
        nums[i] = v;
    }
    put_ints(nums, n);
    put_int(k);
}
