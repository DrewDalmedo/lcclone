/* A number from 1 to 3999, printed as its standard numeral. Small cases
 * use from one to four decimal places and favour the digits 0, 4, 5 and
 * 9; full-size ones pick the shape from the seed: any number, the longest
 * numerals (thousands 3, other digits 3, 7 or 8, up to 15 letters), or
 * numerals full of subtractive pairs (digits 4 and 9). */
static int numeral(int n, char *out)
{
    static const char letters[] = "IVXLCDM";
    int len = 0, place, d, k;
    int digit[3];

    digit[2] = n / 100 % 10;
    digit[1] = n / 10 % 10;
    digit[0] = n % 10;
    for (k = 0; k < n / 1000; k++)
        out[len++] = 'M';
    for (place = 2; place >= 0; place--) {
        char one = letters[2 * place], five = letters[2 * place + 1];
        char ten = letters[2 * place + 2];

        d = digit[place];
        if (d == 9) {
            out[len++] = one;
            out[len++] = ten;
        } else if (d == 4) {
            out[len++] = one;
            out[len++] = five;
        } else {
            if (d >= 5)
                out[len++] = five;
            for (k = 0; k < d % 5; k++)
                out[len++] = one;
        }
    }
    out[len] = '\0';
    return len;
}

static int pick_digit(int small, int shape)
{
    if (small)
        return rnd_chance(50) ? "0459"[rnd(0, 3)] - '0' : (int)rnd(0, 9);
    if (shape == 1)
        return "378"[rnd(0, 2)] - '0';
    if (shape == 2)
        return rnd_chance(80) ? "49"[rnd(0, 1)] - '0' : (int)rnd(0, 9);
    return (int)rnd(0, 9);
}

void gen(int small)
{
    char s[32];
    int shape = small ? 0 : (int)rnd(0, 2);
    int places = small ? (int)rnd(1, 4) : 4, n = 0, p;

    if (!small && shape == 0) {
        n = (int)rnd(1, 3999);
    } else {
        for (p = places - 1; p >= 0; p--) {
            int d = p == 3 ? (shape == 1 ? 3 : (int)rnd(0, 3))
                           : pick_digit(small, shape);

            n = n * 10 + d;
        }
        if (n == 0)
            n = (int)rnd(1, 9);
    }
    numeral(n, s);
    put_string(s);
}
