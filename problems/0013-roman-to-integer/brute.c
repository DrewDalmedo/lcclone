/* Slow and obviously right: write out the standard numeral of every number
 * from 1 to 3999, digit by digit as the statement describes, and return the
 * number whose numeral is s. */
static void write_numeral(int n, char *out)
{
    static const char letters[] = "IVXLCDM";
    int len = 0, place, d, k;

    for (k = 0; k < n / 1000; k++)
        out[len++] = 'M';
    for (place = 2; place >= 0; place--) {
        char one = letters[2 * place], five = letters[2 * place + 1];
        char ten = letters[2 * place + 2];

        d = place == 2 ? n / 100 % 10 : place == 1 ? n / 10 % 10 : n % 10;
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
}

int romanToInt(char *s)
{
    char numeral[32];
    int n;

    for (n = 1; n <= 3999; n++) {
        write_numeral(n, numeral);
        if (strcmp(numeral, s) == 0)
            return n;
    }
    return 0;
}
