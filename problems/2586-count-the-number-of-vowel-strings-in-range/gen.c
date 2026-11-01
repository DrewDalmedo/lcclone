/* Words whose first and last letters are vowels or consonants about half
 * the time each ('y' among the consonants), so about a quarter qualify;
 * an entry sometimes repeats an earlier one. Small cases: up to 8 words of
 * 1 to 4 letters from "aoyk", and any range. Full-size cases: 1000 words
 * (or any number up to 1000) of up to 10 letters, all 10 letters long for
 * some seeds, and the whole array, a random range or a single entry. */
static char random_end(void)
{
    if (rnd_chance(50))
        return rnd_char("aeiou");
    return rnd_char("bcdfghjklmnpqrstvwxyz");
}

void gen(int small)
{
    static char buf[1000][11];
    char *words[1000];
    int n, len, shape, left, right, i, j;

    if (small) {
        n = (int)rnd(1, 8);
        for (i = 0; i < n; i++) {
            if (i > 0 && rnd_chance(15)) {
                j = (int)rnd(0, i - 1);
                strcpy(buf[i], buf[j]);
            } else {
                len = (int)rnd(1, 4);
                for (j = 0; j < len; j++)
                    buf[i][j] = rnd_char("aoyk");
                buf[i][len] = '\0';
            }
            words[i] = buf[i];
        }
        left = (int)rnd(0, n - 1);
        right = (int)rnd(left, n - 1);
    } else {
        shape = (int)(gen_seed() % 3);
        n = rnd_chance(75) ? 1000 : (int)rnd(1, 1000);
        for (i = 0; i < n; i++) {
            if (i > 0 && rnd_chance(5)) {
                j = (int)rnd(0, i - 1);
                strcpy(buf[i], buf[j]);
            } else {
                len = shape == 0 ? 10 : (int)rnd(1, 10);
                buf[i][0] = random_end();
                for (j = 1; j < len - 1; j++)
                    buf[i][j] = rnd_char("abcdefghijklmnopqrstuvwxyz");
                if (len > 1)
                    buf[i][len - 1] = random_end();
                buf[i][len] = '\0';
            }
            words[i] = buf[i];
        }
        if (shape <= 1 && rnd_chance(60)) {
            left = 0;
            right = n - 1;
        } else if (rnd_chance(20)) {
            left = (int)rnd(0, n - 1);
            right = left;
        } else {
            left = (int)rnd(0, n - 1);
            right = (int)rnd(left, n - 1);
        }
    }
    put_strings(words, n);
    put_int(left);
    put_int(right);
}
