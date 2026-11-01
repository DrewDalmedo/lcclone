/* A bag size and a number of seats. The seed picks the shape of the bag:
 * exactly 1 + 2 + ... + k sweets (the last helping is full), 1 to k sweets
 * more (a partial helping), one sweet fewer, or any size. Small cases have
 * up to 8 seats and k up to 11 (at most 77 sweets). Full-size ones have k
 * from 30000 to 44720 (about 4.5 * 10^8 to 10^9 sweets), any bag up to
 * 10^9, or one within 1000 of 10^9, with 1, 2, 999 or 1000 seats or any
 * number up to 1000, so the partial helping can land on any seat. */
static long long tri(long long k)
{
    return k * (k + 1) / 2;
}

void gen(int small)
{
    long long kmin = small ? 1 : 30000, kmax = small ? 11 : 44720, k, c;
    int n, pick;

    if (small) {
        n = (int)rnd(1, 8);
    } else {
        pick = (int)rnd(0, 5);
        n = pick == 0 ? 1 : pick == 1 ? 2 : pick == 2 ? 999 : pick == 3 ? 1000
                                                               : (int)rnd(1, 1000);
    }
    k = rnd(kmin, kmax);
    switch ((int)rnd(0, 4)) {
    case 0: /* the last helping is full */
        c = tri(k);
        break;
    case 1: /* a partial helping of 1 to k sweets after k full ones */
        c = tri(k) + rnd(1, k);
        break;
    case 2: /* one sweet short of filling helping k */
        c = tri(k) - 1;
        break;
    case 3:
        c = small ? rnd(1, 77) : rnd(1, 1000000000);
        break;
    default:
        c = small ? rnd(1, 4) : 1000000000 - rnd(0, 1000);
    }
    if (c < 1)
        c = 1;
    if (c > 1000000000)
        c = 1000000000;
    put_int(c);
    put_int(n);
}
