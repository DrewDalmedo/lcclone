/* n from 1 to 30 when small; at full size 250, the most allowed, half the
 * time, and otherwise anything from 200 to 249. */
void gen(int small)
{
    int n;

    if (small)
        n = (int)rnd(1, 30);
    else if (rnd_chance(50))
        n = 250;
    else
        n = (int)rnd(200, 249);
    put_int(n);
}
