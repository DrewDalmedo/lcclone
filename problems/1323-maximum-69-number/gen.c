/* A number of 6s and 9s: 1 to 4 digits for small cases, 4 for full-size
 * ones. Now and then every digit is a 9, the case where nothing changes. */
void gen(int small)
{
    int digits = small ? (int)rnd(1, 4) : 4, all_nines, num = 0, i, d;

    all_nines = rnd_chance(20);
    for (i = 0; i < digits; i++) {
        if (all_nines || rnd_chance(50))
            d = 9;
        else
            d = 6;
        num = num * 10 + d;
    }
    put_int(num);
}
