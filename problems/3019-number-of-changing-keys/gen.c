/* Letters typed in runs on one key, each letter in a random case. Small
 * cases: up to 10 letters on two or three keys (often a and z, the ends
 * of the alphabet), in runs of up to 3, so that one key in both cases,
 * and keys coming back, show up often. Full-size cases have 100 letters,
 * in a shape the seed picks:
 *   0: any letters;
 *   1: no two neighbours on one key (99 changes, the most there can be);
 *   2: one key throughout (no change);
 *   3: runs of random lengths on a few keys. */
static char typed(int key)
{
    return (char)(rnd_chance(50) ? 'A' + key : 'a' + key);
}

void gen(int small)
{
    char s[101];
    int n = small ? (int)rnd(1, 10) : 100;
    int shape = small ? 3 : (int)(gen_seed() % 4);
    int keys[3], nkeys, cap, len, run, key = -1, k, i;

    keys[0] = rnd_chance(30) ? 0 : (int)rnd(0, 25);
    keys[1] = rnd_chance(30) ? 25 : (int)rnd(0, 25);
    keys[2] = (int)rnd(0, 25);
    nkeys = small ? (int)rnd(2, 3) : 3;
    cap = small ? 3 : (int)rnd(2, 12);
    if (shape == 0) {
        for (len = 0; len < n; len++)
            s[len] = typed((int)rnd(0, 25));
    } else if (shape == 1) {
        for (len = 0; len < n; len++) {
            do
                k = (int)rnd(0, 25);
            while (k == key);
            key = k;
            s[len] = typed(key);
        }
    } else if (shape == 2) {
        key = (int)rnd(0, 25);
        for (len = 0; len < n; len++)
            s[len] = typed(key);
    } else {
        len = 0;
        while (len < n) {
            run = (int)rnd(1, cap);
            key = keys[rnd(0, nkeys - 1)];
            for (i = 0; i < run && len < n; i++)
                s[len++] = typed(key);
        }
    }
    s[n] = '\0';
    put_string(s);
}
