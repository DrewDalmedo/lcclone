/* Builds nums from piles of chosen sizes, then shuffles it. The values
 * are distinct ones from 0..100, with 0 and 100 moved to the front often.
 * Small cases: a few piles of sizes up to 9 (all non-prime for one shape,
 * so false comes up as often as true), or a few values drawn at random.
 * Full size: 100 entries, all distinct; non-prime piles; one prime pile
 * (often 97 or 89) among others; random draws from a few values; or one
 * value 91, 98, 99 or 100 times. */
static int isPrime(int f)
{
    int d;

    if (f < 2)
        return 0;
    for (d = 2; d * d <= f; d++)
        if (f % d == 0)
            return 0;
    return 1;
}

static void front(int *vals, int v)
{
    int i, pos = (int)rnd(0, 2), t;

    for (i = 0; vals[i] != v; i++)
        ;
    t = vals[pos];
    vals[pos] = vals[i];
    vals[i] = t;
}

void gen(int small)
{
    static const int nonPrime[] = {1, 4, 6, 8, 9};
    static const int bigPrime[] = {97, 89, 53, 2, 3};
    static const int equalLen[] = {91, 98, 99, 100};
    int vals[101], nums[100];
    int n = 0, piles = 0, shape, size, left, i, k;

    for (i = 0; i <= 100; i++)
        vals[i] = i;
    rnd_shuffle(vals, 101);
    if (rnd_chance(30))
        front(vals, 0);
    if (rnd_chance(30))
        front(vals, 100);

    if (small) {
        shape = (int)rnd(0, 2);
        if (shape == 0) {
            n = (int)rnd(1, 12);
            k = (int)rnd(1, 4);
            for (i = 0; i < n; i++)
                nums[i] = vals[rnd(0, k - 1)];
        } else {
            piles = (int)rnd(1, 4);
            for (k = 0; k < piles; k++) {
                size = shape == 1 ? nonPrime[rnd(0, 4)] : (int)rnd(1, 9);
                for (i = 0; i < size; i++)
                    nums[n++] = vals[k];
            }
        }
    } else {
        shape = (int)rnd(0, 4);
        if (shape == 0) {
            for (n = 0; n < 100; n++)
                nums[n] = vals[n];
        } else if (shape == 1 || shape == 2) {
            if (shape == 2) {
                size = bigPrime[rnd(0, 4)];
                for (i = 0; i < size; i++)
                    nums[n++] = vals[piles];
                piles++;
            }
            while (n < 100) {
                left = 100 - n;
                size = left < 4 ? 1 : (int)rnd(1, left < 20 ? left : 20);
                if (isPrime(size))
                    continue;
                for (i = 0; i < size; i++)
                    nums[n++] = vals[piles];
                piles++;
            }
        } else if (shape == 3) {
            k = (int)rnd(1, 30);
            for (n = 0; n < 100; n++)
                nums[n] = vals[rnd(0, k - 1)];
        } else {
            size = equalLen[rnd(0, 3)];
            for (n = 0; n < size; n++)
                nums[n] = vals[0];
        }
    }
    rnd_shuffle(nums, n);
    put_ints(nums, n);
}
