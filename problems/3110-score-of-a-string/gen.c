/* The seed picks a shape. Small cases have 2 to 8 letters: from the whole
 * alphabet, from a few letters (so repeats and big jumps come up often), or
 * a walk that moves by at most two letters a step (so equal neighbours and
 * runs that only go up or only go down come up often). Full-size cases have
 * 100 letters: random ones; 'a' and 'z' in turn (the largest score, 2475);
 * one letter repeated (score 0); or long runs up and down the alphabet. */
void gen(int small)
{
    static const char few[] = "abmyz";
    char s[101];
    int n = small ? (int)rnd(2, 8) : 100;
    int shape = (int)rnd(0, small ? 2 : 3);
    int i, c, step, dir;

    if (shape == 0) {
        for (i = 0; i < n; i++)
            s[i] = (char)rnd('a', 'z');
    } else if (shape == 1 && small) {
        for (i = 0; i < n; i++)
            s[i] = rnd_char(few);
    } else if (shape == 1) {
        c = (int)rnd(0, 1);
        for (i = 0; i < n; i++)
            s[i] = (i + c) % 2 ? 'z' : 'a';
    } else if (shape == 2 && small) {
        c = (int)rnd(0, 25);
        for (i = 0; i < n; i++) {
            step = (int)rnd(-2, 2);
            if (c + step >= 0 && c + step <= 25)
                c += step;
            s[i] = (char)('a' + c);
        }
    } else if (shape == 2) {
        c = (int)rnd('a', 'z');
        for (i = 0; i < n; i++)
            s[i] = (char)c;
    } else {
        c = (int)rnd(0, 25);
        dir = rnd_chance(50) ? 1 : -1;
        for (i = 0; i < n; i++) {
            s[i] = (char)('a' + c);
            if (c + dir < 0 || c + dir > 25 || rnd_chance(5))
                dir = -dir;
            step = (int)rnd(0, 2);
            c += dir * step;
            if (c < 0)
                c = 0;
            if (c > 25)
                c = 25;
        }
    }
    s[n] = '\0';
    put_string(s);
}
