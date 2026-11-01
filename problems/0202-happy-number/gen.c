/* Small cases: a random number of 1 to 10 digits, so one-digit numbers come
 * up often. Full-size cases have ten digits, in a shape the seed picks: any
 * such number, a happy one, a happy one that needs 7 steps (the most any n
 * in range needs, about 1 in 160 ten-digit numbers), or one just below
 * 2^31 - 1. */
static long long digit_square_sum(long long n)
{
    long long sum = 0;

    while (n > 0) {
        sum += (n % 10) * (n % 10);
        n /= 10;
    }
    return sum;
}

/* The number of steps n takes to reach 1, or -1 if it never does (the
 * same 1000-step argument as brute.c). */
static int steps_to_one(long long n)
{
    int k;

    for (k = 0; k < 1000; k++) {
        if (n == 1)
            return k;
        n = digit_square_sum(n);
    }
    return -1;
}

void gen(int small)
{
    const long long top = 2147483647;
    long long n = 1;
    int shape, tries, k;

    if (small) {
        int digits = (int)rnd(1, 10), i;
        long long lo = 1;

        for (i = 1; i < digits; i++)
            lo *= 10;
        put_int(rnd(lo, digits == 10 ? top : 10 * lo - 1));
        return;
    }
    shape = (int)rnd(0, 3);
    if (shape == 3) {
        put_int(top - rnd(0, 999));
        return;
    }
    for (tries = 0; tries < 20000; tries++) {
        n = rnd(1000000000, top);
        k = steps_to_one(n);
        if (shape == 0 || (shape == 1 && k >= 0) || (shape == 2 && k == 7))
            break;
    }
    put_int(n);
}
