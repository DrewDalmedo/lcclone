/* Small cases: 1 to 24 codes, each a "W" with a chance of 0, 20, 50 or 80
 * percent and otherwise one of the other eight codes, so the tenth wicket
 * falls in some cases and not in others. Full size: 1000 codes, in a
 * shape the seed picks: no "W" at all; exactly 9 of them; exactly 10, the
 * last one at the very end; about 1 in 100 codes a "W"; every code "6"
 * (6000 runs); mostly "WD" with a rare "W"; or a "W" 3 times in 10, so
 * the innings ends early. */
static char *codes[9] = {"0", "1", "2", "3", "4", "6", "WD", "NB", "W"};

static char *other(void)
{
    return codes[rnd(0, 7)];
}

/* Puts exactly k wickets at distinct random indices below limit. */
static void place(char **ev, int limit, int k)
{
    int i;

    while (k > 0) {
        i = (int)rnd(0, limit - 1);
        if (ev[i] != codes[8]) {
            ev[i] = codes[8];
            k--;
        }
    }
}

void gen(int small)
{
    static const int chances[4] = {0, 20, 50, 80};
    static char *ev[1000];
    int n = small ? (int)rnd(1, 24) : 1000, i, shape, chance = 0;

    shape = small ? -1 : (int)rnd(0, 6);
    if (small)
        chance = chances[rnd(0, 3)];
    else if (shape == 3)
        chance = 1;
    else if (shape == 6)
        chance = 30;
    for (i = 0; i < n; i++) {
        if (shape == 4)
            ev[i] = codes[5];
        else if (shape == 5)
            ev[i] = rnd_chance(70) ? codes[6] : other();
        else if (rnd_chance(chance))
            ev[i] = codes[8];
        else
            ev[i] = other();
    }
    if (shape == 1) {
        place(ev, n, 9);
    } else if (shape == 2) {
        place(ev, n - 1, 9);
        ev[n - 1] = codes[8];
    } else if (shape == 5) {
        place(ev, n, (int)rnd(0, 9));
    }
    put_strings(ev, n);
}
