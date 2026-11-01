/* A loop of shelters. Small cases have up to 8 stretches of length 0 to
 * 10, zeros often, and start == destination now and then. Full-size ones
 * have 10^4 stretches, either all 10^4 long (the longest loop) or of
 * random lengths; the seed puts start and destination at the two ends,
 * half the loop apart, or anywhere. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 10000, i;
    int *distance = malloc((size_t)n * sizeof *distance);
    int all_max = !small && rnd_chance(30);
    int start, destination, ends;

    for (i = 0; i < n; i++) {
        if (all_max)
            distance[i] = 10000;
        else if (small && rnd_chance(25))
            distance[i] = 0;
        else
            distance[i] = (int)rnd(small ? 1 : 0, small ? 10 : 10000);
    }
    start = (int)rnd(0, n - 1);
    if (n == 1 || rnd_chance(10))
        destination = start;
    else
        destination = (start + (int)rnd(1, n - 1)) % n;
    if (!small) {
        ends = (int)rnd(0, 3);
        if (ends == 0) {
            start = 0;
            destination = n - 1;
        } else if (ends == 1) {
            start = n - 1;
            destination = 0;
        } else if (ends == 2) {
            destination = (start + n / 2) % n;
        }
    }
    put_ints(distance, n);
    put_int(start);
    put_int(destination);
    free(distance);
}
