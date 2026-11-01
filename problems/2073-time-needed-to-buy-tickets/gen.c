/* Small cases: 1 to 8 fans wanting 1 to 5 tickets each, so ties with fan k
 * on both sides are common. Full-size cases have 100 fans; the seed picks
 * random wants from 1 to 100, wants from 50 to 100 with k near the back,
 * or the slowest shape for a second-by-second run: every fan wants 100
 * and k is the last fan (10^4 seconds). */
void gen(int small)
{
    int tickets[100], n = small ? (int)rnd(1, 8) : 100, k, shape, i;

    shape = small ? 0 : (int)rnd(0, 2);
    for (i = 0; i < n; i++) {
        if (small)
            tickets[i] = (int)rnd(1, 5);
        else if (shape == 0)
            tickets[i] = (int)rnd(1, 100);
        else if (shape == 1)
            tickets[i] = (int)rnd(50, 100);
        else
            tickets[i] = 100;
    }
    if (shape == 2)
        k = n - 1;
    else if (shape == 1)
        k = (int)rnd(n - 10, n - 1);
    else
        k = (int)rnd(0, n - 1);
    put_ints(tickets, n);
    put_int(k);
}
