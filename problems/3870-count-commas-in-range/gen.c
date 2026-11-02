/* One n. Small cases pick a shape: a value just around 1000, where the
 * first comma appears, any value below 1000 (the answer is 0), or a
 * value up to 20000. Full-size ones are close to 10^5, often exactly
 * 10^5, the largest n allowed. */
void gen(int small)
{
    int shape = (int)rnd(0, 2), n;

    if (small) {
        if (shape == 0)
            n = (int)rnd(990, 1010);
        else if (shape == 1)
            n = (int)rnd(1, 999);
        else
            n = (int)rnd(1000, 20000);
    } else {
        n = shape == 0 ? 100000 : (int)rnd(90000, 100000);
    }
    put_int(n);
}
