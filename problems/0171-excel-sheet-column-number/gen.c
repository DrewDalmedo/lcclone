/* A column name. Small cases have one to four letters, mostly A, B, M, Y
 * and Z, so that names made only of Z and names mixing A and Z come up
 * often. Full-size ones have six or seven letters, Z-heavy or uniform, and
 * a seven-letter name is drawn again until it is no later than FXSHRXW
 * (column 2^31 - 1); or they name a column just below 2^31 - 1. */
void gen(int small)
{
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int shape = (int)rnd(0, small ? 2 : 3), len, i;
    char s[8];

    if (shape == 3) {
        long long n = 2147483647LL - rnd(0, 1000);

        len = 0;
        while (n > 0) {
            n--;
            s[len++] = (char)('A' + n % 26);
            n /= 26;
        }
        for (i = 0; i < len / 2; i++) {
            char t = s[i];

            s[i] = s[len - 1 - i];
            s[len - 1 - i] = t;
        }
        s[len] = '\0';
        put_string(s);
        return;
    }
    do {
        len = small ? (int)rnd(1, 4) : (int)rnd(6, 7);
        for (i = 0; i < len; i++) {
            if (shape == 0)
                s[i] = rnd_char("ABMYZ");
            else if (shape == 1)
                s[i] = rnd_chance(60) ? 'Z' : rnd_char(alphabet);
            else
                s[i] = rnd_char(alphabet);
        }
        s[len] = '\0';
    } while (len == 7 && strcmp(s, "FXSHRXW") > 0);
    put_string(s);
}
