/* The two coin counts. Small cases keep both low, so that either count can
 * run out first and short games are common; full-size ones go up to 100,
 * with the seed picking random counts, counts near the top (games of up
 * to 25 turns), or x equal to y / 4. */
void gen(int small)
{
    int x, y, shape;

    if (small) {
        x = (int)rnd(1, 8);
        y = (int)rnd(1, 36);
    } else {
        shape = (int)rnd(0, 2);
        if (shape == 0) {
            x = (int)rnd(1, 100);
            y = (int)rnd(1, 100);
        } else if (shape == 1) {
            x = (int)rnd(20, 100);
            y = (int)rnd(85, 100);
        } else {
            y = (int)rnd(4, 100);
            x = y / 4;
        }
    }
    put_int(x);
    put_int(y);
}
