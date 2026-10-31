/* A number whose last few digits are 9, so that carries of every length
 * come up, and now and then a number of nothing but 9s. Small cases have
 * up to 5 digits (values below 10^5, so brute.c can count up to them);
 * full-size ones have 100 digits, with a long run of 9s at the end or all
 * 9s, so the carry crosses most of the number. */
void gen(int small)
{
    int digits[100];
    int n = small ? (int)rnd(1, 5) : 100, nines, i;

    if (rnd_chance(small ? 20 : 30))
        nines = n;
    else if (small)
        nines = (int)rnd(0, n - 1);
    else
        nines = (int)rnd(0, rnd_chance(50) ? 99 : 5);
    for (i = 0; i < n; i++)
        digits[i] = (int)rnd(0, small ? 9 : 8);
    for (i = n - nines; i < n; i++)
        digits[i] = 9;
    if (n > 1 && digits[0] == 0)
        digits[0] = (int)rnd(1, 9);
    put_ints(digits, n);
}
