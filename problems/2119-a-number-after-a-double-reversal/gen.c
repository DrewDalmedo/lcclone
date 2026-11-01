/* Builds num digit by digit, with zeros common after the first digit (in
 * the middle and at the end). Small cases have 1 to 4 digits; full-size
 * ones have 6, or are 10^6, the only 7-digit value allowed. */
void gen(int small)
{
    int digits, i, d, num = 0;

    if (!small && rnd_chance(20)) {
        put_int(1000000);
        return;
    }
    digits = small ? (int)rnd(1, 4) : 6;
    for (i = 0; i < digits; i++) {
        if (i == 0)
            d = (int)rnd(digits == 1 ? 0 : 1, 9);
        else
            d = rnd_chance(40) ? 0 : (int)rnd(1, 9);
        num = num * 10 + d;
    }
    put_int(num);
}
