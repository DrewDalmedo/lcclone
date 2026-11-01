/* Random fees. Small queues (at most 7 people, as brute.c searches every
 * arrangement) often use fees 1 to 5, so ties come up; full-size ones have
 * 100 people. The seed may sort the fees up or down, or put the only
 * cheapest person at the back. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 7) : 100, i, j, t;
    int shape = (int)rnd(0, 5);
    int top = small && shape < 3 ? 5 : 100;
    int cost[100];

    for (i = 0; i < n; i++)
        cost[i] = (int)rnd(1, top);
    if (shape == 3 || shape == 4) {
        for (i = 1; i < n; i++)
            for (j = i; j > 0 && (shape == 3 ? cost[j - 1] > cost[j]
                                             : cost[j - 1] < cost[j]); j--) {
                t = cost[j];
                cost[j] = cost[j - 1];
                cost[j - 1] = t;
            }
    } else if (shape == 5) {
        for (i = 0; i + 1 < n; i++)
            cost[i] = (int)rnd(2, top);
        cost[n - 1] = 1;
    }
    put_ints(cost, n);
}
