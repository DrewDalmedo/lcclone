/* Random intervals around a random query minute. Small cases use a few
 * students and minutes up to 12, so intervals often start, end or sit
 * exactly at queryTime. Full-size ones have 100 students over minutes 1 to
 * 1000, with queryTime at 1, at 1000 or anywhere, and the seed may make
 * every student busy then. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100;
    int top = small ? (int)rnd(3, 12) : 1000;
    int q, i, a, b, t, all = 0;
    int *start = malloc((size_t)n * sizeof *start);
    int *end = malloc((size_t)n * sizeof *end);

    t = (int)rnd(0, 3);
    if (small || t >= 2)
        q = (int)rnd(1, top);
    else
        q = t == 0 ? 1 : top;
    if (!small)
        all = rnd_chance(25);
    for (i = 0; i < n; i++) {
        t = all ? -1 : (int)rnd(0, 9);
        if (t == -1) {
            a = (int)rnd(1, q);
            b = (int)rnd(q, top);
        } else if (t == 0) {
            a = q;
            b = q;
        } else if (t == 1) {
            a = q;
            b = (int)rnd(q, top);
        } else if (t == 2) {
            a = (int)rnd(1, q);
            b = q;
        } else {
            a = (int)rnd(1, top);
            b = (int)rnd(1, top);
            if (a > b) {
                t = a;
                a = b;
                b = t;
            }
        }
        start[i] = a;
        end[i] = b;
    }
    put_ints(start, n);
    put_ints(end, n);
    put_int(q);
    free(start);
    free(end);
}
