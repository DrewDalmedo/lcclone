/* One n. The seed picks the shape:
 *   - any n (up to 999 for small cases, up to 10^6 otherwise);
 *   - a single digit, or a two-digit number ending in 9 (t equals n);
 *   - a power of ten (t = 1), 10^6 included;
 *   - a number with some 0 digits, so that the product is 0;
 *   - the first n from a random start whose answer is true, with or
 *     without a 0 digit (true answers without a 0 are rare).
 * Small cases stay below 10^4, apart from the powers of ten. */
static int key(int n)
{
    int sum = 0, prod = 1;

    while (n > 0) {
        sum += n % 10;
        prod *= n % 10;
        n /= 10;
    }
    return sum + prod;
}

static int has_zero(int n)
{
    while (n > 0) {
        if (n % 10 == 0)
            return 1;
        n /= 10;
    }
    return 0;
}

void gen(int small)
{
    int shape = (int)rnd(0, 6), n, len, i, d, zeros;

    if (shape == 0) {
        n = (int)rnd(1, small ? 999 : 1000000);
    } else if (shape == 1) {
        if (rnd_chance(50))
            n = (int)rnd(1, 9);
        else
            n = 10 * (int)rnd(1, 9) + 9;
    } else if (shape == 2) {
        len = (int)rnd(0, 6);
        for (n = 1, i = 0; i < len; i++)
            n *= 10;
    } else if (shape == 3) {
        len = small ? (int)rnd(2, 4) : (int)rnd(4, 6);
        n = (int)rnd(1, 9);
        zeros = 0;
        for (i = 1; i < len; i++) {
            d = rnd_chance(40) ? 0 : (int)rnd(1, 9);
            if (i == len - 1 && zeros == 0)
                d = 0;
            if (d == 0)
                zeros++;
            n = n * 10 + d;
        }
    } else {
        /* Zero-free true answers go up to 999324, so start below it. */
        n = (int)rnd(1, small ? 5000 : 990000);
        while (n % key(n) != 0 || (shape >= 5 && has_zero(n)))
            n++;
    }
    put_int(n);
}
