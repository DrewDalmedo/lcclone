/* Random words and weights. Small cases have a few short words over the
 * letters a to e and z, with weights either anywhere in 1 to 100 or drawn
 * from values near multiples of 26 (1, 13, 25, 26, 27, 52, 100), so that
 * remainders 0 and 25 come up; words may repeat. Full-size cases have 100
 * words of up to 10 letters over the whole alphabet. */
void gen(int small)
{
    static const int near[] = {1, 13, 25, 26, 27, 52, 100};
    char *words[100];
    char buf[100][11];
    int weights[26];
    int n = small ? (int)rnd(1, 5) : 100, i, j, len, special;

    special = small && rnd_chance(50);
    for (i = 0; i < 26; i++)
        weights[i] = special ? near[rnd(0, 6)] : (int)rnd(1, 100);
    for (i = 0; i < n; i++) {
        if (small && i > 0 && rnd_chance(20)) {
            j = (int)rnd(0, i - 1);
            strcpy(buf[i], buf[j]);
        } else {
            len = small ? (int)rnd(1, 4) : (int)rnd(1, 10);
            for (j = 0; j < len; j++)
                buf[i][j] = small ? rnd_char("abcdez")
                                  : rnd_char("abcdefghijklmnopqrstuvwxyz");
            buf[i][len] = '\0';
        }
        words[i] = buf[i];
    }
    put_strings(words, n);
    put_ints(weights, 26);
}
