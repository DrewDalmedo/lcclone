/* A perfect square k^2 (twice in six); one less than a square; a number
 * strictly between k^2 and (k + 1)^2, often k * (k + 1); any number; or a
 * number near an edge. Small cases keep k at most 44 (num below 2025) and
 * often use tiny values. Full-size cases are near the top of the int
 * range, where squaring a guess in 32 bits overflows, in a shape the seed
 * picks: 2^31 - 1 (no square, so trying every k up to num never stops
 * early); 46340^2 = 2147395600, the largest square; one below it; one
 * above it; the square of a k from 32768 to 46340; or a number within
 * 10^5 of 2^31 - 1 that is no square. */
static long long full_size(void)
{
    static const long long edge[] = {
        2147483647LL, 2147395600LL, 2147395599LL, 2147395601LL
    };
    int shape = (int)(gen_seed() % 6);
    long long k;

    if (shape < 4)
        return edge[shape];
    if (shape == 4) {
        k = rnd(32768, 46340);
        return k * k;
    }
    /* Above 46340^2 + 10^4 and below 2^31: no square lies there. */
    return 2147483647LL - rnd(0, 70000);
}

void gen(int small)
{
    long long kmin = 1, kmax = 44, k, num;
    long long top = 2147483647;

    if (!small) {
        put_int(full_size());
        return;
    }
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
        num = rnd(1, 2024);
        break;
    default:
        num = rnd(1, 10);
    }
    put_int(num);
}
