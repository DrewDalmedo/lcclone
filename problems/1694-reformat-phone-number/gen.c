/* A phone number of D digits with spaces and dashes put in random places:
 * at either end, between digits, in mixed runs. Small cases have 2 to 14
 * digits, often many zeros, and up to 8 separators. Full-size ones are
 * 100 characters long; the seed picks all digits (the longest output, 133
 * characters), or a random D from 2 to 100. */
void gen(int small)
{
    char s[101], digit_set[11];
    int slot[100], d, len, i;

    if (small) {
        d = (int)rnd(2, 14);
        len = d + (int)rnd(0, 8);
    } else {
        len = 100;
        d = rnd_chance(50) ? 100 : (int)rnd(2, 100);
    }
    strcpy(digit_set, small && rnd_chance(30) ? "00001" : "0123456789");
    for (i = 0; i < len; i++)
        slot[i] = i < d;
    rnd_shuffle(slot, len);
    for (i = 0; i < len; i++) {
        if (slot[i])
            s[i] = rnd_char(digit_set);
        else
            s[i] = rnd_char(" -");
    }
    s[len] = '\0';
    put_string(s);
}
