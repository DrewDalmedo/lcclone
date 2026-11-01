/* A temperature with at most two decimals: a whole number of hundredths,
 * divided by 100. Small cases stay below 100 degrees and are often whole
 * or have a single decimal. */
void gen(int small)
{
    long long c100 = rnd(0, small ? 10000 : 100000);
    int r = (int)rnd(0, 2);

    if (small && r == 0)
        c100 -= c100 % 100;
    else if (small && r == 1)
        c100 -= c100 % 10;
    put_double((double)c100 / 100);
}
