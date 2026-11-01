/* A number whose digits are often 0, so runs of zeros, trailing zeros and
 * numbers with no zeros all come up; sometimes 0 itself. Small cases have
 * up to 6 digits; full size ones have 9, or are 10^9 itself. */
void gen(int small)
{
    int len = small ? (int)rnd(1, 6) : 9, zeros = (int)rnd(20, 70), i;
    int n = (int)rnd(1, 9);

    if (rnd_chance(5)) {
        put_int(small ? 0 : 1000000000);
        return;
    }
    for (i = 1; i < len; i++)
        n = n * 10 + (rnd_chance(zeros) ? 0 : (int)rnd(1, 9));
    put_int(n);
}
