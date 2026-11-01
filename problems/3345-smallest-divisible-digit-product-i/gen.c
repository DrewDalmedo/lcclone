/* Any n and t within the constraints. A full-size case leans towards the
 * longest searches: n just past a multiple of 10, or near the top, with a
 * t that few digit products meet. */
void gen(int small)
{
    static const int hard_t[] = {7, 9, 10, 8};
    int n, t;

    if (small || rnd_chance(30)) {
        n = (int)rnd(1, 100);
        t = (int)rnd(1, 10);
    } else {
        int tens = (int)rnd(0, 9);

        n = tens * 10 + (int)rnd(1, 9);
        t = hard_t[rnd(0, 3)];
    }
    put_int(n);
    put_int(t);
}
