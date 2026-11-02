/* A number and a digit. Small cases: n is 0, 100000, or 1 to 5 digits
 * long, each digit being x with a chance of 40 in 100 (the leading digit
 * too, unless x is 0), so that x turns up first, later, both or not at
 * all. Full-size cases: n from 10000 to 100000, and x a random digit of
 * n half the time, else any digit. */
void gen(int small)
{
    int x = (int)rnd(0, 9), n = 0, len, i, d, pick;
    int digits[6];

    if (!small) {
        n = (int)rnd(10000, 100000);
        if (rnd_chance(50)) {
            for (len = 0, d = n; d > 0; d /= 10)
                digits[len++] = d % 10;
            x = digits[rnd(0, len - 1)];
        }
        put_int(n);
        put_int(x);
        return;
    }
    pick = (int)rnd(0, 19);
    if (pick == 0) {
        n = 0;
    } else if (pick == 1) {
        n = 100000;
    } else {
        len = (int)rnd(1, 5);
        for (i = 0; i < len; i++) {
            d = rnd_chance(40) ? x : (int)rnd(0, 9);
            if (i == 0 && len > 1 && d == 0)
                d = (int)rnd(1, 9);
            n = n * 10 + d;
        }
    }
    put_int(n);
    put_int(x);
}
