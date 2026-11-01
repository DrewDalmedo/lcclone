/* Small cases are often the first few n, where 1 and 2 are special;
 * otherwise any n allowed. Full-size cases sit near the top bound, 100. */
void gen(int small)
{
    int n;

    if (small)
        n = rnd_chance(30) ? (int)rnd(1, 4) : (int)rnd(1, 100);
    else
        n = (int)rnd(90, 100);
    put_int(n);
}
