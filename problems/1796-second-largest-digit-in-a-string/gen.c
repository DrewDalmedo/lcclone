/* Small cases are up to 8 characters: letters from b, j, z and digits from
 * a small pool the seed picks (only 0, only 7, 0 and 9, 8 and 9, ..., or
 * all ten), with a share of digits from none to all, so that no digit, one
 * repeated digit and an answer of 0 come up often. Full-size cases are 500
 * characters, in a shape the seed picks: mixed; digits only; letters
 * only; one digit repeated among letters; the biggest digit repeated
 * with the second one only at the very end; or digits only at both ends. */
static const char letters[] = "abcdefghijklmnopqrstuvwxyz";
static const char digits[] = "0123456789";

void gen(int small)
{
    static const char *pools[] = {"0", "7", "09", "38", "89", "012", "4579",
                                  "0123456789", "0123456789"};
    static const int percent[] = {0, 50, 75, 100};
    char s[501], one[2], big, low;
    const char *pool;
    int n = small ? (int)rnd(1, 8) : 500, i, shape, share, digit;

    if (small) {
        share = percent[rnd(0, 3)];
        pool = pools[rnd(0, 8)];
        for (i = 0; i < n; i++) {
            digit = rnd_chance(share);
            s[i] = rnd_char(digit ? pool : "bjz");
        }
    } else {
        shape = (int)(gen_seed() % 6);
        big = (char)('0' + rnd(1, 9));
        low = (char)('0' + rnd(0, big - '0' - 1));
        one[0] = shape == 3 ? (char)('0' + rnd(0, 9)) : big;
        one[1] = '\0';
        for (i = 0; i < n; i++) {
            if (shape == 0) {
                digit = rnd_chance(50);
                s[i] = rnd_char(digit ? digits : letters);
            } else if (shape == 1) {
                s[i] = rnd_char(digits);
            } else if (shape == 2) {
                s[i] = rnd_char(letters);
            } else if (shape == 3 || shape == 4) {
                digit = rnd_chance(50);
                s[i] = rnd_char(digit ? one : letters);
            } else {
                digit = i < 3 || i >= n - 3;
                s[i] = rnd_char(digit ? digits : letters);
            }
        }
        if (shape == 4)
            s[n - 1] = low;
    }
    s[n] = '\0';
    put_string(s);
}
