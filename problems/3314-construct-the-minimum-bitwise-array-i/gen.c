/* Small cases have 1 to 6 primes: mostly primes below 100, sometimes one
 * from a short list of edge primes (2, 3, the Mersenne primes 7, 31 and
 * 127, primes with long runs of trailing 1 bits such as 191, 223, 383, 479
 * and 991, and the largest, 997), so that 2s and repeats come up often.
 * Full-size cases have 100 primes, with the seed picking the shape: any
 * primes up to 1000; any primes with every edge prime among them; or a few
 * primes repeated many times. */
static const int edge[] = {2, 3, 7, 31, 127, 191, 383, 991, 997, 223, 479};

void gen(int small)
{
    int primes[168], pool[8], a[100];
    char composite[1001] = {0};
    int count = 0, below100 = 0, n, shape, i, k, j;

    for (i = 2; i <= 1000; i++) {
        if (composite[i])
            continue;
        primes[count++] = i;
        if (i < 100)
            below100 = count;
        for (j = i * i; j <= 1000; j += i)
            composite[j] = 1;
    }
    n = small ? (int)rnd(1, 6) : 100;
    shape = small ? 0 : (int)rnd(0, 2);
    if (small) {
        for (i = 0; i < n; i++) {
            k = rnd_chance(25);
            if (k) {
                j = (int)rnd(0, 10);
                a[i] = edge[j];
            } else {
                j = (int)rnd(0, below100 - 1);
                a[i] = primes[j];
            }
        }
    } else if (shape == 0) {
        for (i = 0; i < n; i++) {
            j = (int)rnd(0, count - 1);
            a[i] = primes[j];
        }
    } else if (shape == 1) {
        for (i = 0; i < 11; i++)
            a[i] = edge[i];
        for (i = 11; i < n; i++) {
            j = (int)rnd(0, count - 1);
            a[i] = primes[j];
        }
        rnd_shuffle(a, n);
    } else {
        k = (int)rnd(2, 8);
        for (i = 0; i < k; i++) {
            j = (int)rnd(0, count - 1);
            pool[i] = primes[j];
        }
        for (i = 0; i < n; i++) {
            j = (int)rnd(0, k - 1);
            a[i] = pool[j];
        }
    }
    put_ints(a, n);
}
