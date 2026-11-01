/* Any position. Small cases lean to the first few positions, where the
 * two starting numbers and the first sums are; full-size ones to the end
 * of the range, n from 25 to 30, where a plain recursion makes the most
 * calls. */
void gen(int small)
{
    int n;

    if (small)
        n = rnd_chance(50) ? (int)rnd(0, 6) : (int)rnd(0, 30);
    else
        n = rnd_chance(50) ? 30 : (int)rnd(25, 30);
    put_int(n);
}
