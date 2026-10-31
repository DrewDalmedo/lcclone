/* Two numbers; small ones for the cross-check. */
void gen(int small)
{
    int top = small ? 20 : 1000;

    put_int(rnd(-top, top));
    put_int(rnd(-top, top));
}
