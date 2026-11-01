/* Small cases: n and start up to 12, so that every remainder of each
 * by 4 comes up, with start often 0 to 3. Full size: n and start
 * anywhere in their ranges, each at its largest value now and then. */
void gen(int small)
{
    int n, start;

    if (small) {
        n = (int)rnd(1, 12);
        start = rnd_chance(40) ? (int)rnd(0, 3) : (int)rnd(0, 12);
    } else {
        n = rnd_chance(25) ? 1000 : (int)rnd(1, 1000);
        start = rnd_chance(25) ? 1000 : (int)rnd(0, 1000);
    }
    put_int(n);
    put_int(start);
}
