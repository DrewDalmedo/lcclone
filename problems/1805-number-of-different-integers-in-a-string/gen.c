/* Small cases are up to 10 characters: letters p and q, and digits from a
 * pool the seed picks (only 0; 0 and 1; mostly 0 with 1 and 2; any digit),
 * with a share of digits from none to all, so that numbers with leading
 * zeros that stand for the same integer come up often. Full-size cases are
 * 1000 characters, in a shape the seed picks: mixed; all digits (one
 * number); many short numbers; a few numbers of over 200 digits that
 * differ, if at all, only in the last digit and in leading zeros; numbers
 * made only of zeros, with an occasional 1; or numbers that share their
 * last 25 digits and differ only before them. */
static const char letters[] = "abcdefghijklmnopqrstuvwxyz";
static const char digits[] = "0123456789";

static void put(char *s, int *pos, char c)
{
    if (*pos < 1000)
        s[(*pos)++] = c;
}

void gen(int small)
{
    static const char *pools[] = {"0", "01", "00012", "0123456789"};
    static const int percent[] = {0, 50, 75, 100};
    char s[1001], base[229], tail[25], c;
    const char *pool;
    int n = small ? (int)rnd(1, 10) : 1000, pos = 0, i, k, len, share, shape;
    int digit;

    if (small) {
        share = percent[rnd(0, 3)];
        pool = pools[rnd(0, 3)];
        for (i = 0; i < n; i++) {
            digit = rnd_chance(share);
            s[i] = rnd_char(digit ? pool : "pq");
        }
        pos = n;
    } else {
        shape = (int)(gen_seed() % 6);
        for (i = 0; i < 229; i++)
            base[i] = rnd_char(digits);
        base[0] = rnd_char("123456789");
        for (i = 0; i < 25; i++)
            tail[i] = rnd_char(digits);
        while (pos < n) {
            if (shape == 0) {
                digit = rnd_chance(60);
                c = rnd_char(digit ? digits : letters);
                put(s, &pos, c);
            } else if (shape == 1) {
                c = rnd_char(digits);
                put(s, &pos, c);
            } else if (shape == 2) {
                len = (int)rnd(1, 3);
                for (i = 0; i < len; i++) {
                    c = rnd_char(digits);
                    put(s, &pos, c);
                }
                c = rnd_char(letters);
                put(s, &pos, c);
            } else if (shape == 3) {
                k = (int)rnd(0, 3);
                if (pos + k + 231 > n) {
                    /* No room for another long number: letters to the end. */
                    c = rnd_char(letters);
                    put(s, &pos, c);
                    continue;
                }
                for (i = 0; i < k; i++)
                    put(s, &pos, '0');
                for (i = 0; i < 229; i++)
                    put(s, &pos, base[i]);
                c = rnd_char("12");
                put(s, &pos, c);
                c = rnd_char(letters);
                put(s, &pos, c);
            } else if (shape == 4) {
                len = (int)rnd(1, 5);
                digit = rnd_chance(10);
                for (i = 0; i < len; i++)
                    put(s, &pos, digit && i == len - 1 ? '1' : '0');
                c = rnd_char(letters);
                put(s, &pos, c);
            } else {
                len = (int)rnd(1, 3);
                for (i = 0; i < len; i++) {
                    c = rnd_char("0123");
                    put(s, &pos, c);
                }
                for (i = 0; i < 25; i++)
                    put(s, &pos, tail[i]);
                len = (int)rnd(1, 2);
                for (i = 0; i < len; i++) {
                    c = rnd_char(letters);
                    put(s, &pos, c);
                }
            }
        }
    }
    s[pos] = '\0';
    put_string(s);
}
