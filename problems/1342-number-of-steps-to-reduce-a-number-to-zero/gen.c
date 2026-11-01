/* One number. Small cases are below 64, and 0 or 1 a sixth of the time.
 * Full-size ones pick a shape by seed: any number up to 10^6, one just
 * below 10^6, a power of two, a run of 1 bits (2^p - 1), or one of the
 * three numbers up to 10^6 with 19 of 20 bits set, which take the most
 * steps (38). */
void gen(int small)
{
    int shape, p, num;

    if (small) {
        num = rnd_chance(17) ? (int)rnd(0, 1) : (int)rnd(0, 63);
        put_int(num);
        return;
    }
    shape = (int)rnd(0, 4);
    if (shape == 0) {
        num = (int)rnd(0, 1000000);
    } else if (shape == 1) {
        num = 1000000 - (int)rnd(0, 1000);
    } else if (shape == 2) {
        p = (int)rnd(0, 19);
        num = 1 << p;
    } else if (shape == 3) {
        p = (int)rnd(1, 19);
        num = (1 << p) - 1;
    } else {
        p = (int)rnd(16, 18);
        num = (1 << 20) - 1 - (1 << p);
    }
    put_int(num);
}
