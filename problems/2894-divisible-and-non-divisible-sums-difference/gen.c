/* Two numbers n and m. The seed picks how m relates to n: m = 1 (every
 * number is a multiple), m = n, m above n (no multiples), a small m with
 * many multiples, or any m up to n. Small cases keep n <= 30; full-size
 * ones take n from 900 to 1000, often exactly 1000. */
void gen(int small)
{
    int n, m, top, shape = (int)rnd(0, 4);

    if (small)
        n = (int)rnd(1, 30);
    else
        n = rnd_chance(50) ? 1000 : (int)rnd(900, 1000);
    top = small ? n + 10 : 1000;
    if (shape == 0)
        m = 1;
    else if (shape == 1)
        m = n;
    else if (shape == 2)
        m = n < top ? (int)rnd(n + 1, top) : n;
    else if (shape == 3)
        m = (int)rnd(2, small ? 4 : 40);
    else
        m = (int)rnd(1, n);
    put_int(n);
    put_int(m);
}
