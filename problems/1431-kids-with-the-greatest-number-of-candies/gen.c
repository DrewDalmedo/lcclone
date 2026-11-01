/* Small cases: 2 to 8 children holding 1 to 10 candies and a bag of 1 to
 * 5, so that ties for the most and totals landing exactly on it are
 * common. Full-size cases: 100 children; the seed picks counts spread over
 * 1 to 100 (mixed answers), counts packed within the bag's size of each
 * other (all true), or everyone equal. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 100, shape = (int)rnd(0, 2), i, v;
    int extra = small ? (int)rnd(1, 5) : (int)rnd(1, 50);
    int low = (int)rnd(1, 100 - extra), same = (int)rnd(1, 100);
    int candies[100];

    for (i = 0; i < n; i++) {
        if (small)
            v = (int)rnd(1, 10);
        else if (shape == 0)
            v = (int)rnd(1, 100);
        else if (shape == 1)
            v = (int)rnd(low, low + extra);
        else
            v = same;
        candies[i] = v;
    }
    put_ints(candies, n);
    put_int(extra);
}
