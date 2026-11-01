/* Small cases have 1 to 7 seats and students, at positions 1 to 4, 1 to
 * 8, 93 to 100 or 1 to 100, so that shared positions and students already
 * at a seat come up often; sometimes the students stand exactly where the
 * seats are, in another order.
 * Full-size cases have 100 of each, in a shape the seed picks: random;
 * every seat at 1 and every student at 100 (the largest answer, 9900);
 * the students on the seats' positions, shuffled (0); or seats crowded
 * at the left and students at the right. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 7) : 100, i, shape, range, lo, hi;
    int *seats = malloc((size_t)n * sizeof *seats);
    int *students = malloc((size_t)n * sizeof *students);

    if (small) {
        range = (int)rnd(0, 3);
        lo = range == 1 ? 93 : 1;
        hi = range == 0 ? 8 : range == 3 ? 4 : 100;
        shape = rnd_chance(15) ? 2 : 0;
    } else {
        lo = 1;
        hi = 100;
        shape = (int)(gen_seed() % 4);
    }
    for (i = 0; i < n; i++) {
        if (shape == 1) {
            seats[i] = 1;
            students[i] = 100;
        } else if (shape == 3) {
            seats[i] = (int)rnd(1, 30);
            students[i] = (int)rnd(60, 100);
        } else {
            seats[i] = (int)rnd(lo, hi);
            students[i] = (int)rnd(lo, hi);
        }
    }
    if (shape == 2) {
        for (i = 0; i < n; i++)
            students[i] = seats[i];
        rnd_shuffle(students, n);
    }
    put_ints(seats, n);
    put_ints(students, n);
    free(seats);
    free(students);
}
