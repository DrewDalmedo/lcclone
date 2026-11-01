/* A four-digit number. Most cases take their digits from a pool of three,
 * one of which is 0, so that zeros and repeated digits come up often; the
 * rest are any number from 1000 to 9999. Every input is tiny, so small
 * and full-size cases are made alike. */
void gen(int small)
{
    int pool[3], num, i, k;

    (void)small;
    if (rnd_chance(35)) {
        put_int(rnd(1000, 9999));
        return;
    }
    pool[0] = 0;
    pool[1] = (int)rnd(1, 9);
    pool[2] = (int)rnd(1, 9);
    k = (int)rnd(1, 2);
    num = pool[k];
    for (i = 1; i < 4; i++) {
        k = (int)rnd(0, 2);
        num = num * 10 + pool[k];
    }
    put_int(num);
}
