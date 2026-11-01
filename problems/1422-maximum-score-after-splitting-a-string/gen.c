/* A string of 0s and 1s, of a shape the seed picks: random characters;
 * mostly 0s, then mostly 1s (the best cut lands in the middle and scores
 * near the length); a single repeated character (cuts at either end);
 * alternating 0 and 1; or mostly 1s then mostly 0s (low scores, best cut
 * at an end). Small cases have 2 to 8 characters, full-size ones 500. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 500, shape = (int)rnd(0, 4), i;
    int noise = (int)rnd(0, 15), odd = (int)rnd(0, 1);
    char c, fill = rnd_char("01"), s[501];

    for (i = 0; i < n; i++) {
        if (shape == 0)
            c = rnd_char("01");
        else if (shape == 1)
            c = (i < n / 2) == !rnd_chance(noise) ? '0' : '1';
        else if (shape == 2)
            c = fill;
        else if (shape == 3)
            c = i % 2 == odd ? '1' : '0';
        else
            c = (i < n / 2) == !rnd_chance(noise) ? '1' : '0';
        s[i] = c;
    }
    s[n] = '\0';
    put_string(s);
}
