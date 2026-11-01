/* A perfect number or one of its neighbours; a number 2^(p-1) * (2^p - 1),
 * which is perfect only when 2^p - 1 is prime; a power of two, whose total
 * is one short; a prime, whose total is 1; a multiple of 720720, with
 * many divisors; or any number. Small cases keep num at most 10^4; full
 * size ones reach 10^8 and lean to the top of the range, where testing
 * every smaller number takes longest. */
static int is_prime(long long n)
{
    long long d;

    if (n < 2)
        return 0;
    for (d = 2; d * d <= n; d++)
        if (n % d == 0)
            return 0;
    return 1;
}

void gen(int small)
{
    static const long long perfect[] = {6, 28, 496, 8128, 33550336};
    long long top = small ? 10000 : 100000000, num;
    int p;

    switch ((int)rnd(0, 6)) {
    case 0:
        num = perfect[rnd(0, small ? 3 : 4)];
        if (rnd_chance(40))
            num += rnd_chance(50) ? 1 : -1;
        break;
    case 1:
        p = (int)rnd(1, small ? 7 : 13);
        num = (1LL << (p - 1)) * ((1LL << p) - 1);
        break;
    case 2:
        p = (int)rnd(0, small ? 13 : 26);
        num = 1LL << p;
        break;
    case 3:
        num = rnd(small ? 2 : top / 2, top);
        while (!is_prime(num))
            num--;
        break;
    case 4:
        num = small ? rnd(1, 30) : 720720 * rnd(1, top / 720720);
        break;
    case 5:
        num = small ? rnd(1, top) : rnd(top - 1000000, top);
        break;
    default:
        num = rnd(1, top);
    }
    put_int(num);
}
