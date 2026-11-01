/* A random num. Small cases have 2 to 7 digits (now and then just 1),
 * drawn from 0 up to a cap of 3 to 9 that the seed picks, so digits repeat
 * and zeros come up often. Full-size ones have 9 digits, by a shape the
 * seed picks: random digits, or both families rising from left to right in
 * random places (so most digits have to move); or num is 10^9, the only
 * input with 10 digits. */
void gen(int small)
{
    char d[11];
    int n, top, shape, i, v, odd, even;

    if (small) {
        n = rnd_chance(10) ? 1 : (int)rnd(2, 7);
        top = (int)rnd(3, 9);
        for (i = 0; i < n; i++) {
            v = (int)rnd(i == 0 ? 1 : 0, top);
            d[i] = (char)('0' + v);
        }
        d[n] = '\0';
        put_raw(d);
        return;
    }
    shape = (int)rnd(0, 3);
    if (shape == 3) {
        put_int(1000000000);
        return;
    }
    n = 9;
    if (shape == 0) {
        for (i = 0; i < n; i++) {
            v = (int)rnd(i == 0 ? 1 : 0, 9);
            d[i] = (char)('0' + v);
        }
    } else {
        /* Each place is odd or even at random; the odd places get odd
         * digits and the even places even digits, each family never going
         * down from left to right. */
        odd = 1;
        even = shape == 1 ? 0 : 2;
        for (i = 0; i < n; i++) {
            if (rnd_chance(50) || (i == 0 && even == 0)) {
                d[i] = (char)('0' + odd);
                if (odd < 9 && rnd_chance(70))
                    odd += 2;
            } else {
                d[i] = (char)('0' + even);
                if (even < 8 && rnd_chance(70))
                    even += 2;
            }
        }
    }
    d[n] = '\0';
    put_raw(d);
}
