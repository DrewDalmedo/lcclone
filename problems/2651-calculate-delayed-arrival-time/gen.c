/* A scheduled hour and a delay. A third of the time the sum is 23, 24 or
 * 25, right around midnight; otherwise both are drawn at random from their
 * whole ranges (small and full-size cases are alike). */
void gen(int small)
{
    int arrival, delay, sum, lo, hi;

    (void)small;
    if (rnd_chance(33)) {
        sum = (int)rnd(23, 25);
        lo = sum - 24 > 1 ? sum - 24 : 1; /* delay <= 24 */
        hi = sum - 1 < 23 ? sum - 1 : 23; /* delay >= 1 */
        arrival = (int)rnd(lo, hi);
        delay = sum - arrival;
    } else {
        arrival = (int)rnd(1, 23);
        delay = (int)rnd(1, 24);
    }
    put_int(arrival);
    put_int(delay);
}
