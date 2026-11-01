/* A perfect square k^2 (twice in six); one less than a square; a number
 * strictly between k^2 and (k + 1)^2, often k * (k + 1); any number; or a
 * number near an edge. Small cases keep k at most 44 (num below 2025) and
 * often use tiny values. Full-size cases keep k from 32768 to 46340 (num
 * from 2^30 - 1 up to 2^31 - 1), where squaring a guess in 32 bits
 * overflows and counting k up takes the most steps; their edges are
 * 46339^2, 46340^2 = 2147395600 (the largest square) and its neighbours,
 * 2^31 - 1, or anything within 10^5 of it. */
void gen(int small)
{
    long long kmin = small ? 1 : 32768, kmax = small ? 44 : 46340, k, num;
    long long top = 2147483647;

    switch ((int)rnd(0, 5)) {
    case 0:
    case 1:
        k = rnd(kmin, kmax);
        num = k * k;
        break;
    case 2:
        k = rnd(kmin < 2 ? 2 : kmin, kmax);
        num = k * k - 1;
        break;
    case 3:
        k = rnd(kmin, kmax);
        if (rnd_chance(50))
            num = k * (k + 1);
        else
            num = k * k + rnd(1, 2 * k);
        if (num > top)
            num = top - rnd(0, top - k * k - 1);
        break;
    case 4:
        num = small ? rnd(1, 2024) : rnd(1073741823, top);
        break;
    default:
        if (small) {
            num = rnd(1, 10);
        } else if (rnd_chance(50)) {
            static const long long edge[] = {
                2147302921LL, 2147395599LL, 2147395600LL, 2147395601LL,
                2147483647LL
            };

            num = edge[rnd(0, 4)];
        } else {
            num = top - rnd(0, 100000);
        }
    }
    put_int(num);
}
