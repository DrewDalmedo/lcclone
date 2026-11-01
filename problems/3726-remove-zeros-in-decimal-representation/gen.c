/* A number whose digits are often 0, so runs of zeros, trailing zeros and
 * numbers with no zeros all come up. Small cases have up to 8 digits; full
 * size ones have 15, or are 10^15 itself. */
void gen(int small)
{
    int len = small ? (int)rnd(1, 8) : 15, zeros = (int)rnd(20, 70), i;
    long long n = (long long)rnd(1, 9);

    if (!small && rnd_chance(5)) {
        put_int(1000000000000000LL);
        return;
    }
    for (i = 1; i < len; i++)
        n = n * 10 + (rnd_chance(zeros) ? 0 : rnd(1, 9));
    put_int(n);
}
