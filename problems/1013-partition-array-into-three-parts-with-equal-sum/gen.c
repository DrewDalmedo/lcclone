/* The seed picks a shape: random values; three pieces built to share a sum
 * (sometimes nudged off by one); a zero-sum front followed by a rest that
 * also sums to 0, so the prefix sums come back to 0 at the last element
 * (where a cut would leave the back empty); values of -1, 0 and 1, whose
 * prefix sums hit the third many times; and, at full size only, a slow
 * false case: the front is the third and the prefix sums then swing
 * between the third and another value, never reaching two thirds before
 * the last element. Small cases have 3 to 9 elements from -3 to 3. */

/* Nudges a[from..to) from its end, within [-top, top], to sum to target.
 * It reaches the target when |target| <= (to - from) * top. */
static void fit(int *a, int from, int to, long long target, int top)
{
    long long sum = 0, d, room, step;
    int k;

    for (k = from; k < to; k++)
        sum += a[k];
    d = target - sum;
    for (k = to - 1; k >= from && d != 0; k--) {
        room = d > 0 ? top - a[k] : -top - a[k];
        step = d > 0 ? (d < room ? d : room) : (d > room ? d : room);
        a[k] += (int)step;
        d -= step;
    }
}

void gen(int small)
{
    int n = small ? (int)rnd(3, 9) : 50000;
    int top = small ? 3 : 10000;
    int shape = (int)rnd(0, small ? 3 : 5);
    int *a = malloc((size_t)n * sizeof *a);
    int i, j, k, x, len, delta;
    long long t;

    for (k = 0; k < n; k++)
        a[k] = (int)rnd(-top, top);
    if (shape == 1) {
        /* Front a[0..i], middle a[i+1..j-1], back a[j..n-1]. */
        i = (int)rnd(0, n - 3);
        j = (int)rnd(i + 2, n - 1);
        if (!small && rnd_chance(30))
            i = 0;
        if (!small && rnd_chance(30))
            j = n - 1;
        len = i + 1;
        if (j - i - 1 < len)
            len = j - i - 1;
        if (n - j < len)
            len = n - j;
        t = rnd(-(long long)len * top, (long long)len * top);
        fit(a, 0, i + 1, t, top);
        fit(a, i + 1, j, t, top);
        fit(a, j, n, t, top);
        if (rnd_chance(30)) {
            k = (int)rnd(0, n - 1);
            delta = rnd_chance(50) ? 1 : -1;
            if (a[k] + delta >= -top && a[k] + delta <= top)
                a[k] += delta;
        }
    } else if (shape == 2) {
        i = (int)rnd(0, n - 3);
        fit(a, 0, i + 1, 0, top);
        fit(a, i + 1, n, 0, top);
    } else if (shape == 3) {
        for (k = 0; k < n; k++)
            a[k] = (int)rnd(-1, 1);
    } else if (shape == 4) {
        t = rnd(1, top / 2);
        if (rnd_chance(50))
            t = -t;
        a[0] = (int)t;
        for (k = 1; k + 1 < n - 1; k += 2) {
            do
                x = (int)rnd(-top, top);
            while (x == 0 || x == t);
            a[k] = x;
            a[k + 1] = -x;
        }
        a[n - 1] = (int)(2 * t);
    }
    put_ints(a, n);
    free(a);
}
