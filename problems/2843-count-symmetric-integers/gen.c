/* Small cases: a short range (up to 150 numbers) somewhere in 1..10^4, or
 * one around a change in the count of digits (9/10, 99/100, 999/1000,
 * 9999/10000), and sometimes any range at all. Full-size cases reach from
 * near 1 to near 10^4: the whole of 1..10^4 for one seed in four. */
void gen(int small)
{
    static const int edge[] = {1, 10, 11, 99, 100, 999, 1000, 1001, 9999, 10000};
    int shape, low, high, t;

    if (small) {
        shape = (int)rnd(0, 3);
        if (shape == 0) {
            t = edge[rnd(0, 9)];
            low = t - (int)rnd(0, 60);
            high = t + (int)rnd(0, 60);
        } else if (shape == 3) {
            low = (int)rnd(1, 10000);
            high = (int)rnd(1, 10000);
        } else {
            low = (int)rnd(1, 10000);
            high = low + (int)rnd(0, 150);
        }
        if (low > high) {
            t = low;
            low = high;
            high = t;
        }
        if (low < 1)
            low = 1;
        if (high > 10000)
            high = 10000;
    } else if (gen_seed() % 4 == 0) {
        low = 1;
        high = 10000;
    } else {
        low = (int)rnd(1, 100);
        high = (int)rnd(9900, 10000);
    }
    put_int(low);
    put_int(high);
}
