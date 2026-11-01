/* A row index. Small cases are half the time a row up to 8 and otherwise
 * any row up to 33: brute.c builds the whole triangle cheaply, so the
 * cross-check also reaches the rows whose numbers pass 10^9. Full-size
 * ones are from 25 up, half the time the largest, 33. */
void gen(int small)
{
    if (small)
        put_int(rnd_chance(50) ? rnd(0, 8) : rnd(9, 33));
    else
        put_int(rnd_chance(50) ? 33 : rnd(25, 32));
}
