/* Labels come from a pool: in small cases 2 to 4 labels (rarely 1), mostly
 * different ones from 1 to 9 (sometimes from the whole range up to 10^5); in
 * full-size cases a pool of 1 to 10 labels or a fresh label for every push.
 * The seed picks the push times. Small cases (1 to 8 pushes): times of 1
 * to 4, so ties are common; all equal, so every push ties; times of 1 or 2
 * with one long push first, last or anywhere (sometimes taking the record
 * to second 10^5); or times of 1 to 12. Full-size cases (mostly 1000
 * pushes): distinct random seconds from 1 to 10^5; all equal, as long as
 * the range allows; one second each but one long push, first, last or
 * anywhere, ending the record at 10^5; or times of 1 to 3. */
void gen(int small)
{
    static int rows[2000], pool[1000];
    int n, k, shape = (int)rnd(0, 3), i, at = -1, big = 0, d = 1, wide;
    long long t = 0, need, left, v;

    if (small) {
        n = (int)rnd(1, 8);
        k = rnd_chance(10) ? 1 : (int)rnd(2, 4);
        wide = rnd_chance(15);
        for (i = 0; i < 9; i++)
            pool[i] = i + 1;
        rnd_shuffle(pool, 9);
        for (i = 0; wide && i < k; i++)
            pool[i] = (int)rnd(1, 100000);
    } else {
        n = rnd_chance(75) ? 1000 : (int)rnd(1, 1000);
        k = rnd_chance(50) ? (int)rnd(1, 10) : n;
        for (i = 0; i < k; i++)
            pool[i] = (int)rnd(1, 100000);
    }
    for (i = 0; i < n; i++)
        rows[2 * i] = pool[rnd(0, k - 1)];

    if (shape == 1)
        d = small ? (int)rnd(1, 5) : 100000 / n;
    if (shape == 2) {
        i = (int)rnd(0, 2);
        at = i == 0 ? 0 : i == 1 ? n - 1 : (int)rnd(0, n - 1);
        if (!small || rnd_chance(20))
            big = -1; /* worked out below: ends the record at 10^5 */
        else
            big = (int)rnd(2, 9);
    }
    if (!small && shape == 0) {
        /* n distinct seconds from 1 to 10^5, in order (selection sampling). */
        need = n;
        i = 0;
        for (v = 1; v <= 100000 && need > 0; v++) {
            left = 100000 - v + 1;
            if (rnd(0, left - 1) < need) {
                rows[2 * i + 1] = (int)v;
                i++;
                need--;
            }
        }
    } else {
        for (i = 0; i < n; i++) {
            if (shape == 0)
                t += rnd(1, 4);
            else if (shape == 1)
                t += d;
            else if (shape == 2) {
                if (i != at)
                    t += small ? rnd(1, 2) : 1;
                else if (big > 0)
                    t += big;
                else
                    t += 1; /* placeholder, fixed up below */
            } else
                t += small ? rnd(1, 12) : rnd(1, 3);
            rows[2 * i + 1] = (int)t;
        }
        if (shape == 2 && big < 0) {
            /* Stretch push `at` so that the last push lands at 10^5. */
            d = 100000 - rows[2 * n - 1];
            for (i = at; i < n; i++)
                rows[2 * i + 1] += d;
        }
    }
    put_int_grid(rows, n, 2);
}
