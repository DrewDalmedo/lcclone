/* Small cases: up to 8 statements of the four forms. Full size: 100
 * statements, raising X with a chance the seed picks (never, rarely, half
 * the time, mostly or always), so that results near -100, 0 and 100 all
 * come up. */
void gen(int small)
{
    static char *forms[4] = {"++X", "X++", "--X", "X--"};
    char *ops[100];
    int n = small ? (int)rnd(1, 8) : 100, up, i;
    static const int chances[5] = {0, 10, 50, 90, 100};
    int chance = small ? 50 : chances[rnd(0, 4)];

    for (i = 0; i < n; i++) {
        up = rnd_chance(chance);
        ops[i] = forms[2 * !up + (int)rnd(0, 1)];
    }
    put_strings(ops, n);
}
