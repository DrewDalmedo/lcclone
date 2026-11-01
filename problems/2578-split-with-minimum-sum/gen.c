/* A number of 2 to 9 digits, or 10^9, the only one with 10. Small cases
 * have 2 to 6 digits, mostly drawn from a pool of three, one of them 0, so
 * that zeros and repeated digits come up often. Full-size cases have 7 to
 * 9 digits: nine different digits in a random order (the most orders for
 * a slow method to try), or any digits; now and then num is 10^9 or
 * 999999999, which give the smallest and the largest answer. */
static void put_digits(const int *d, int k)
{
    long long num = 0;
    int i;

    for (i = 0; i < k; i++)
        num = num * 10 + d[i];
    put_int(num);
}

void gen(int small)
{
    int d[10], pool[3], k, i, j, t, pick;

    if (small) {
        if (rnd_chance(4)) {
            put_int(1000000000);
            return;
        }
        k = (int)rnd(2, 6);
        if (rnd_chance(25)) {
            d[0] = (int)rnd(1, 9);
            for (i = 1; i < k; i++)
                d[i] = (int)rnd(0, 9);
        } else {
            pool[0] = 0;
            pool[1] = (int)rnd(1, 9);
            pool[2] = (int)rnd(1, 9);
            j = (int)rnd(1, 2);
            d[0] = pool[j];
            for (i = 1; i < k; i++) {
                j = (int)rnd(0, 2);
                d[i] = pool[j];
            }
        }
        put_digits(d, k);
        return;
    }
    pick = (int)rnd(0, 9);
    if (pick == 0) {
        put_int(1000000000);
    } else if (pick == 1) {
        put_int(999999999);
    } else if (pick <= 5) {
        for (i = 0; i < 10; i++)
            d[i] = i;
        rnd_shuffle(d, 10);
        if (d[0] == 0) {
            t = d[0];
            d[0] = d[9];
            d[9] = t;
        }
        put_digits(d, 9);
    } else {
        k = (int)rnd(7, 9);
        d[0] = (int)rnd(1, 9);
        for (i = 1; i < k; i++)
            d[i] = (int)rnd(0, 9);
        put_digits(d, k);
    }
}
