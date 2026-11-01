/* The seed picks a shape: colors drawn at random, all houses alike but
 * one, or one color at both ends with a few odd houses inside. Small cases
 * have 2 to 8 houses and colors 0 to 3; full-size cases have 100 houses
 * and colors 0 to 100. Whenever every house comes out alike (always, in
 * the second shape), one house at a random place gets another color, so
 * two colors always differ. */
void gen(int small)
{
    int colors[100], n = small ? (int)rnd(2, 8) : 100;
    int top = small ? 3 : 100, shape = (int)rnd(0, 2), base, odd, pos, i;

    base = (int)rnd(0, top);
    for (i = 0; i < n; i++) {
        if (shape == 0) {
            colors[i] = (int)rnd(0, top);
        } else if (shape == 2 && i > 0 && i < n - 1 && rnd_chance(15)) {
            odd = (int)rnd(0, top);
            colors[i] = odd;
        } else {
            colors[i] = base;
        }
    }
    for (i = 1; i < n && colors[i] == colors[0]; i++)
        ;
    if (i == n) {
        pos = (int)rnd(0, n - 1);
        odd = (int)rnd(0, top - 1);
        colors[pos] = odd < colors[0] ? odd : odd + 1;
    }
    put_ints(colors, n);
}
