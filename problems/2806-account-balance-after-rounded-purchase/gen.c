/* Any price from 0 to 100; a third of the time one ending in 5, the
 * halfway prices that must round up. There is no larger size to make. */
void gen(int small)
{
    int price;

    (void)small;
    if (rnd_chance(33))
        price = (int)rnd(0, 9) * 10 + 5;
    else
        price = (int)rnd(0, 100);
    put_int(price);
}
