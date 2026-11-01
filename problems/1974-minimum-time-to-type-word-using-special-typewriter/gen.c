/* The seed picks a shape. Small cases have 1 to 8 letters, either from the
 * whole alphabet or from a few letters around the seam between 'z' and
 * 'a' and at the far side of the dial (so repeats, wraps and the distance
 * of exactly 13 come up often). Full-size cases have 100 letters: random
 * ones; pairs 13 apart in turn (the most notches per letter); letters that
 * stay close to each other but far from 'a'; or one letter repeated. */
void gen(int small)
{
    static const char few[] = "abmnoxyz";
    char word[101];
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = (int)rnd(0, small ? 1 : 3);
    int i, c, step;

    if (shape == 0) {
        for (i = 0; i < n; i++)
            word[i] = (char)rnd('a', 'z');
    } else if (shape == 1 && small) {
        for (i = 0; i < n; i++)
            word[i] = rnd_char(few);
    } else if (shape == 1) {
        c = (int)rnd(0, 12);
        for (i = 0; i < n; i++)
            word[i] = (char)('a' + (i % 2 ? c + 13 : c));
    } else if (shape == 2) {
        c = (int)rnd(9, 17);
        for (i = 0; i < n; i++) {
            step = (int)rnd(-2, 2);
            if (c + step >= 8 && c + step <= 18)
                c += step;
            word[i] = (char)('a' + c);
        }
    } else {
        c = (int)rnd('a', 'z');
        for (i = 0; i < n; i++)
            word[i] = (char)c;
    }
    word[n] = '\0';
    put_string(word);
}
