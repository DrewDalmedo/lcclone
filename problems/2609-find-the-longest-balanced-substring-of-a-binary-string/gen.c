/* Small cases have 1 to 10 characters, either random or built from groups
 * of 1 to 3 equal characters, so that short groups and ties come up often.
 * Full-size cases have 50 characters, in a shape the seed picks: random;
 * groups of 1 to 6; one long group of 0s then 1s (lengths equal or not)
 * placed among short groups; all one character; alternating; 1s first,
 * then 0s; or several 0s-then-1s pairs of random sizes. */

/* Fills s[0..n) with groups of 1 to top equal characters, alternating
 * between 0 and 1, starting with first. */
static void groups(char *s, int n, int top, char first)
{
    char c = first;
    int i = 0, len, k;

    while (i < n) {
        len = (int)rnd(1, top);
        for (k = 0; k < len && i < n; k++)
            s[i++] = c;
        c = c == '0' ? '1' : '0';
    }
}

void gen(int small)
{
    char s[51], first;
    int n = small ? (int)rnd(1, 10) : 50, i, k, shape, z, o, at, cut;

    s[n] = '\0';
    first = rnd_char("01");
    if (small) {
        if (rnd_chance(50)) {
            for (i = 0; i < n; i++)
                s[i] = rnd_char("01");
        } else {
            groups(s, n, 3, first);
        }
        put_string(s);
        return;
    }
    shape = (int)(gen_seed() % 7);
    if (shape == 0) {
        for (i = 0; i < n; i++)
            s[i] = rnd_char("01");
    } else if (shape == 1) {
        groups(s, n, 6, first);
    } else if (shape == 2) {
        /* z 0s then o 1s, with a 1 before and a 0 after where they fit. */
        groups(s, n, 3, first);
        z = (int)rnd(5, 24);
        o = rnd_chance(50) ? z : (int)rnd(5, 24);
        at = (int)rnd(0, n - z - o);
        for (k = 0; k < z; k++)
            s[at + k] = '0';
        for (k = 0; k < o; k++)
            s[at + z + k] = '1';
        if (at > 0)
            s[at - 1] = '1';
        if (at + z + o < n)
            s[at + z + o] = '0';
    } else if (shape == 3) {
        for (i = 0; i < n; i++)
            s[i] = first;
    } else if (shape == 4) {
        for (i = 0; i < n; i++)
            s[i] = i % 2 == 0 ? first : (first == '0' ? '1' : '0');
    } else if (shape == 5) {
        cut = (int)rnd(1, n - 1);
        for (i = 0; i < n; i++)
            s[i] = i < cut ? '1' : '0';
    } else {
        /* Pairs of a group of 0s and a group of 1s, each 1 to 12 long. */
        i = 0;
        while (i < n) {
            z = (int)rnd(1, 12);
            o = (int)rnd(1, 12);
            for (k = 0; k < z && i < n; k++)
                s[i++] = '0';
            for (k = 0; k < o && i < n; k++)
                s[i++] = '1';
        }
    }
    put_string(s);
}
