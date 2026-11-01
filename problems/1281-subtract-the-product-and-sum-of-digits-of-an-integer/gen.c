/* One n. Small cases build it digit by digit from 1 to 5 digits, with
 * zeros, ones and nines coming up often, and now and then take the bound
 * 10^5. Full-size ones take any n up to 10^5, five nonzero digits (the
 * largest products), or 10^5 itself. */
static int digit(void)
{
    int pick = (int)rnd(0, 4);

    if (pick == 0)
        return 0;
    if (pick == 1)
        return 1;
    if (pick == 2)
        return 9;
    return (int)rnd(0, 9);
}

void gen(int small)
{
    int shape = (int)rnd(0, 9), len, d, n = 0, i;

    if (shape == 0) {
        put_int(100000);
        return;
    }
    if (!small && shape <= 5) {
        put_int(rnd(1, 100000));
        return;
    }
    len = small ? (int)rnd(1, 5) : 5;
    for (i = 0; i < len; i++) {
        do
            d = small ? digit() : (int)rnd(1, 9);
        while (i == 0 && d == 0);
        n = n * 10 + d;
    }
    put_int(n);
}
