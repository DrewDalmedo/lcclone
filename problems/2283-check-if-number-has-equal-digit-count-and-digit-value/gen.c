/* True strings are rare, so the seed picks a shape: a true string (small
 * cases: one of six, 4 to 10 digits long; full-size ones: the 10-digit
 * one), the same with one digit changed, shuffled, or one digit shorter
 * or longer (full size: one digit taken out and another put at the end);
 * random digits 0 to 3 or 0 to 9; or what a random string says about
 * itself (digit i replaced by the count of digit i), which gets many
 * claims right without being true. Full-size cases have 10 digits. */
static const char *const truths[] = {
    "2020", "21200", "3211000", "42101000", "521001000", "6210001000"
};

void gen(int small)
{
    char s[12], d;
    int count[10], shape = (int)rnd(0, 6), n, i, j, t, removed;

    if (shape <= 3) {
        t = small ? (int)rnd(0, 5) : 5;
        strcpy(s, truths[t]);
        n = (int)strlen(s);
        if (shape == 1) {
            i = (int)rnd(0, n - 1);
            do
                d = rnd_char("0123456789");
            while (d == s[i]);
            s[i] = d;
        } else if (shape == 2) {
            for (i = n - 1; i > 0; i--) {
                j = (int)rnd(0, i);
                d = s[i];
                s[i] = s[j];
                s[j] = d;
            }
        } else if (shape == 3) {
            removed = 0;
            if (!small || n == 10 || rnd_chance(50)) {
                i = (int)rnd(0, n - 1);
                memmove(s + i, s + i + 1, (size_t)(n - i));
                n--;
                removed = 1;
            }
            if (!small || !removed) {
                s[n++] = rnd_char("0123456789");
                s[n] = '\0';
            }
        }
    } else {
        n = small ? (int)rnd(1, 10) : 10;
        for (i = 0; i < n; i++)
            s[i] = shape == 4 ? rnd_char("0123") : rnd_char("0123456789");
        s[n] = '\0';
        if (shape == 6) {
            for (i = 0; i < 10; i++)
                count[i] = 0;
            for (i = 0; i < n; i++)
                count[s[i] - '0']++;
            for (i = 0; i < n; i++)
                s[i] = (char)('0' + (count[i] > 9 ? 9 : count[i]));
        }
    }
    put_string(s);
}
