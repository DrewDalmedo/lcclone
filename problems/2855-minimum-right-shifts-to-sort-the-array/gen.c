/* n distinct values from 1 to top: n is 1 to 8 and top n to n + 10 for
 * small cases; for full-size ones n is 100 (so the values are exactly 1
 * to 100) or, now and then, less, with top 100. The values are sorted and
 * the seed picks what happens next:
 *   - a rotation by a random number of places (the answer);
 *   - a rotation, then two values swapped, mostly next to each other or
 *     across the ends, so that the array is nearly a rotation;
 *   - a rotation, then the first and last values swapped: this keeps a
 *     single drop inside the array but breaks the step from the last
 *     value round to the first;
 *   - a random order. */
void gen(int small)
{
    int pool[100], a[100], b[100];
    int n, top, shape, k, i, j, t;

    n = small ? (int)rnd(1, 8) : (rnd_chance(70) ? 100 : (int)rnd(1, 99));
    top = small ? n + (int)rnd(0, 10) : 100;
    for (i = 0; i < top; i++)
        pool[i] = i + 1;
    for (i = 0; i < n; i++) {
        j = (int)rnd(i, top - 1);
        t = pool[i];
        pool[i] = pool[j];
        pool[j] = t;
    }
    for (i = 1; i < n; i++)
        for (j = i; j > 0 && pool[j - 1] > pool[j]; j--) {
            t = pool[j];
            pool[j] = pool[j - 1];
            pool[j - 1] = t;
        }
    k = (int)rnd(0, n - 1);
    for (i = 0; i < n; i++)
        b[(i + k) % n] = pool[i];
    shape = (int)rnd(0, 3);
    if (shape == 1 && n >= 2) {
        i = (int)rnd(0, n - 1);
        j = rnd_chance(70) ? (i + 1) % n : (int)rnd(0, n - 1);
        t = b[i];
        b[i] = b[j];
        b[j] = t;
    } else if (shape == 2 && n >= 2) {
        t = b[0];
        b[0] = b[n - 1];
        b[n - 1] = t;
    } else if (shape == 3) {
        for (i = 0; i < n; i++)
            a[i] = i;
        rnd_shuffle(a, n);
        for (i = 0; i < n; i++)
            b[i] = pool[a[i]];
    }
    put_ints(b, n);
}
