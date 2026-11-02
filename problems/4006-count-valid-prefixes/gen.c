/* A string of 0s and 1s. The seed picks the shape: random characters,
 * runs of equal characters (so prefixes stop being valid and become valid
 * again), pairs "01" and "10" with a stray character now and then, or one
 * character throughout. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 12) : 100;
    int shape = (int)rnd(0, 7), i = 0, len, k;
    char *s = malloc((size_t)n + 1);
    char c = rnd_char("01");

    while (i < n) {
        if (shape <= 2) {
            s[i++] = rnd_char("01");
        } else if (shape <= 4) {
            len = (int)rnd(1, small ? 4 : 30);
            for (k = 0; k < len && i < n; k++)
                s[i++] = c;
            c = c == '0' ? '1' : '0';
        } else if (shape <= 6) {
            if (rnd_chance(15)) {
                s[i++] = rnd_char("01");
            } else {
                c = rnd_char("01");
                s[i++] = c;
                if (i < n)
                    s[i++] = c == '0' ? '1' : '0';
            }
        } else {
            s[i++] = c;
        }
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
