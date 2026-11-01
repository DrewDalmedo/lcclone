/* A row of chairs and a time. Often the time is a whole number of one-way
 * trips (n - 1 seconds each), give or take a second, so the pillow is at
 * an end of the row or just next to it, which is where wrong periods and
 * missed turns show. Small cases have up to 8 chairs and 40 seconds.
 * Full-size ones have 2, 3, 999 or 1000 chairs or any number up to 1000,
 * and times up to 1000, often exactly 1000. */
void gen(int small)
{
    int n, time, pick, trips, delta, top;

    if (small) {
        n = (int)rnd(2, 8);
        top = 40;
    } else {
        pick = (int)rnd(0, 5);
        n = pick == 0 ? 2 : pick == 1 ? 3 : pick == 2 ? 999 : pick == 3 ? 1000
                                                               : (int)rnd(2, 1000);
        top = 1000;
    }
    pick = (int)rnd(0, 3);
    if (pick <= 1) {
        trips = (int)rnd(1, top / (n - 1) > 1 ? top / (n - 1) : 1);
        delta = (int)rnd(-1, 1);
        time = trips * (n - 1) + delta;
    } else if (pick == 2 && !small) {
        time = 1000;
    } else {
        time = (int)rnd(1, top);
    }
    if (time < 1)
        time = 1;
    if (time > top)
        time = top;
    put_int(n);
    put_int(time);
}
