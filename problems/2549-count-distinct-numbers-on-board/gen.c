/* One n. Small cases are often 1 or 2 (no news is passed on) or below 20,
 * else any n up to 100. Full-size ones take the bound 100 or an n from 90
 * to 100. */
void gen(int small)
{
    int shape = (int)rnd(0, 4), n;

    if (small) {
        if (shape == 0)
            n = (int)rnd(1, 2);
        else if (shape <= 2)
            n = (int)rnd(1, 19);
        else
            n = (int)rnd(1, 100);
    } else {
        n = shape == 0 ? 100 : (int)rnd(90, 100);
    }
    put_int(n);
}
