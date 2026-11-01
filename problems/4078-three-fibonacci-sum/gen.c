/* A number picked by a shape: a sum of three neighbouring terms,
 * 2 * F(k), one such sum moved by 1 or 2 either way, or any number. Small
 * cases stay below 140 (k up to 10); full-size ones reach k = 43, the
 * largest sum below 10^9, and numbers close to 10^9. */
void gen(int small)
{
    long long fib[44], n;
    int top = small ? 10 : 43, shape = (int)rnd(0, small ? 2 : 3), k, d;

    fib[0] = 0;
    fib[1] = 1;
    for (k = 2; k <= 43; k++)
        fib[k] = fib[k - 1] + fib[k - 2];
    k = (int)rnd(2, top);
    if (shape == 0) {
        n = 2 * fib[k];
    } else if (shape == 1) {
        d = (int)rnd(1, 2);
        n = rnd_chance(50) ? 2 * fib[k] + d : 2 * fib[k] - d;
        if (n < 1)
            n = 2 * fib[k] + d;
    } else if (shape == 2) {
        n = small ? rnd(1, 140) : rnd(1, 1000000000);
    } else {
        n = rnd(1000000000 - 100, 1000000000);
    }
    put_int(n);
}
