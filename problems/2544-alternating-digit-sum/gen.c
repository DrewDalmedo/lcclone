/* One n. Small cases build it digit by digit, 1 to 9 digits long, with
 * zeros, ones and nines coming up often, so that both odd and even digit
 * counts, zeros at every place and extreme results appear; now and then
 * they take the bound 10^9. Full-size ones take any n up to 10^9, 8 or 9
 * such digits, or 10^9 itself. */
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
        put_int(1000000000);
        return;
    }
    if (!small && shape <= 5) {
        put_int(rnd(1, 1000000000));
        return;
    }
    len = small ? (int)rnd(1, 9) : (int)rnd(8, 9);
    for (i = 0; i < len; i++) {
        do
            d = digit();
        while (i == 0 && d == 0);
        n = n * 10 + d;
    }
    put_int(n);
}
