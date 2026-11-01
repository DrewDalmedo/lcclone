/* Small cases: up to 8 letters from "abc", and a letter from "abcd", so
 * the letter is often absent, present once, or everywhere. Full-size
 * ones have 100 letters (or, by seed, a random length near 100) with a
 * random number of copies of the letter in random places: by seed, any
 * number, all of them, or just one or all but one. */
void gen(int small)
{
    char s[101], letter, other;
    int n, c, i, j, shape, *pos;

    if (small) {
        n = (int)rnd(1, 8);
        for (i = 0; i < n; i++)
            s[i] = rnd_char("abc");
        s[n] = '\0';
        letter = rnd_char("abcd");
    } else {
        shape = (int)rnd(0, 3);
        n = shape == 1 ? (int)rnd(90, 100) : 100;
        letter = rnd_char("abcdefghijklmnopqrstuvwxyz");
        if (shape == 2)
            c = n;
        else if (shape == 3)
            c = rnd_chance(50) ? 1 : n - 1;
        else
            c = (int)rnd(0, n);
        pos = malloc((size_t)n * sizeof *pos);
        for (i = 0; i < n; i++)
            pos[i] = i;
        rnd_shuffle(pos, n);
        for (i = 0; i < n; i++) {
            do
                other = rnd_char("abcdefghijklmnopqrstuvwxyz");
            while (other == letter);
            s[i] = other;
        }
        for (j = 0; j < c; j++)
            s[pos[j]] = letter;
        s[n] = '\0';
        free(pos);
    }
    put_string(s);
    put_char(letter);
}
