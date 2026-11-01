/* A number of rows: up to 12 for small cases (brute.c's recursion is
 * exponential), and for full-size ones from 20 up, half the time the
 * largest, 30. */
void gen(int small)
{
    if (small)
        put_int(rnd(1, 12));
    else
        put_int(rnd_chance(50) ? 30 : rnd(20, 29));
}
