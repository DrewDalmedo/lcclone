/* Sorted strike times and a duration. Small cases have up to 8 strikes,
 * with gaps drawn from 0 to duration + 2 (so one second short of
 * duration, exactly duration and one second past it all come up), and
 * duration 0 now and then. Full-size cases have 10^4 strikes, in a shape
 * the seed picks: all in one second with the largest duration (the
 * slowest input for walking through every second of every strike);
 * spread evenly from 0 to 10^7 with the largest duration (answers near
 * the largest); random times and duration; gaps around a small
 * duration; or duration 0. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 10000, duration, gap, t, i;
    int *times = malloc((size_t)n * sizeof *times);

    if (small) {
        duration = 0;
        if (!rnd_chance(15))
            duration = (int)rnd(1, 6);
        times[0] = (int)rnd(0, 5);
        for (i = 1; i < n; i++) {
            gap = (int)rnd(0, duration + 2);
            times[i] = times[i - 1] + gap;
        }
    } else {
        switch (gen_seed() % 5) {
        case 0: /* every strike in one second */
            duration = 10000000;
            t = (int)rnd(0, 10000000);
            for (i = 0; i < n; i++)
                times[i] = t;
            break;
        case 1: /* spread evenly from 0 to 10^7 */
            duration = 10000000;
            for (i = 0; i < n; i++)
                times[i] = (int)((long long)i * 10000000 / (n - 1));
            break;
        case 2: /* random times and duration */
            duration = (int)rnd(0, 10000000);
            for (i = 0; i < n; i++)
                times[i] = (int)rnd(0, 10000000);
            qsort(times, (size_t)n, sizeof *times, cmp_int);
            break;
        case 3: /* gaps of duration - 1, duration or duration + 1 */
            duration = (int)rnd(1, 999);
            times[0] = (int)rnd(0, 1000);
            for (i = 1; i < n; i++) {
                gap = (int)rnd(duration - 1, duration + 1);
                times[i] = times[i - 1] + gap;
            }
            break;
        default: /* duration 0 */
            duration = 0;
            for (i = 0; i < n; i++)
                times[i] = (int)rnd(0, 10000000);
            qsort(times, (size_t)n, sizeof *times, cmp_int);
            break;
        }
    }
    put_ints(times, n);
    put_int(duration);
    free(times);
}
