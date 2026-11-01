/* A base k from 2 to 10 and a number n. Small cases often keep n at most
 * k (one digit, or 10 in base k); otherwise n is anything from 1 to 100.
 * Full-size ones take n = 100, the n up to 100 with the largest digit sum
 * in base k (the largest such n when several tie), or n from 90 to 100. */
static int digit_sum(int n, int k)
{
    int sum = 0;

    for (; n > 0; n /= k)
        sum += n % k;
    return sum;
}

void gen(int small)
{
    int k = (int)rnd(2, 10), shape = (int)rnd(0, 2), n, m;

    if (small) {
        if (shape == 0)
            n = (int)rnd(1, k);
        else
            n = (int)rnd(1, 100);
    } else if (shape == 0) {
        n = 100;
    } else if (shape == 1) {
        n = 1;
        for (m = 2; m <= 100; m++)
            if (digit_sum(m, k) >= digit_sum(n, k))
                n = m;
    } else {
        n = (int)rnd(90, 100);
    }
    put_int(n);
    put_int(k);
}
