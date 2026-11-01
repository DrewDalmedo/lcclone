/* Each array leans to 0s or 1s by a random amount. Small cases have up to
 * 8 students; in a fifth of them the pile holds exactly the kinds the line
 * wants, in another order. Full-size cases have 100 students; the seed
 * picks random arrays, a shuffled pile matching the line, or the slow
 * shape for a step-by-step run: a long run of 1s at the front of the line
 * and a pile that alternates 0, 1, 0, 1, ... from the top. */
static void fill(int *a, int n)
{
    int lean = (int)rnd(0, 10) * 10, i;

    for (i = 0; i < n; i++)
        a[i] = rnd_chance(lean);
}

void gen(int small)
{
    int students[100], sandwiches[100], n, shape, ones, i;

    n = small ? (int)rnd(1, 8) : 100;
    shape = small ? (int)rnd(0, 4) : (int)rnd(0, 2);
    fill(students, n);
    if (small ? shape > 0 : shape == 0) {
        fill(sandwiches, n);
    } else if (small || shape == 1) {
        for (i = 0; i < n; i++)
            sandwiches[i] = students[i];
        rnd_shuffle(sandwiches, n);
    } else {
        ones = (int)rnd(40, 60);
        for (i = 0; i < n; i++) {
            students[i] = i < ones;
            sandwiches[i] = i % 2;
        }
    }
    put_ints(students, n);
    put_ints(sandwiches, n);
}
