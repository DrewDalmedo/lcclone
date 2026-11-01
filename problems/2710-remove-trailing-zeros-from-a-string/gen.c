/* A number is a body that starts with a digit from 1 to 9, then trailing
 * zeros. Small cases have a body of 1 to 6 digits, often zeros inside (and
 * so sometimes a body that ends in 0 itself), and 0 to 4 zeros after it.
 * Full-size ones have 1000 digits, and the seed picks how many of them
 * are trailing zeros: none, one, a few, many, or 999, so that only the
 * first digit is left. There the body's last digit is never 0, and about
 * a third of its other digits are zeros. */
void gen(int small)
{
    int n = small ? 0 : 1000, body, trail, shape, i;
    char *num;

    if (small) {
        body = (int)rnd(1, 6);
        trail = rnd_chance(30) ? 0 : (int)rnd(1, 4);
        n = body + trail;
    } else {
        shape = (int)rnd(0, 4);
        if (shape == 0)
            trail = 0;
        else if (shape == 1)
            trail = 1;
        else if (shape == 2)
            trail = (int)rnd(2, 40);
        else if (shape == 3)
            trail = (int)rnd(41, 998);
        else
            trail = 999;
        body = n - trail;
    }
    num = malloc((size_t)n + 1);
    for (i = 0; i < body; i++) {
        if (i == 0 || (!small && i == body - 1))
            num[i] = rnd_char("123456789");
        else
            num[i] = small ? rnd_char("00123456789") : rnd_char("000123456789");
    }
    for (i = body; i < n; i++)
        num[i] = '0';
    num[n] = '\0';
    put_string(num);
    free(num);
}
