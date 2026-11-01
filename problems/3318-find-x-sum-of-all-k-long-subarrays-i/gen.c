/* Small cases have 1 to 8 elements drawn from a few values (1..m with m
 * from 2 to 5, sometimes moved to the top of the range, 46..50), so that
 * counts tie often. Full-size cases have 50 elements, with the seed
 * picking the shape: any values 1..50 (both bounds included); 3 to 10
 * distinct values, so that counts are large and tie; or k == n, a single
 * window. In both, k is random and x is often small (at most 3), so that
 * windows have more distinct values than x. */
void gen(int small)
{
    int a[50];
    int n = small ? (int)rnd(1, 8) : 50;
    int shape = small ? 0 : (int)rnd(0, 2);
    int m, top, k, x, i, v;

    if (small) {
        m = (int)rnd(2, 5);
        top = rnd_chance(30);
        for (i = 0; i < n; i++) {
            v = (int)rnd(1, m);
            a[i] = top ? 51 - v : v;
        }
    } else if (shape == 0) {
        a[0] = 1;
        a[1] = 50;
        for (i = 2; i < n; i++)
            a[i] = (int)rnd(1, 50);
        rnd_shuffle(a, n);
    } else {
        m = (int)rnd(3, 10);
        for (i = 0; i < n; i++)
            a[i] = (int)rnd(1, m) * (50 / m);
    }
    k = shape == 2 ? n : (int)rnd(1, n);
    x = (int)rnd(1, k);
    if (rnd_chance(60))
        x = (int)rnd(1, k < 3 ? k : 3);
    put_ints(a, n);
    put_int(k);
    put_int(x);
}
