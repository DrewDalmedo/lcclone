/* A string s and a letter c that occurs in it. Small cases have up to 12
 * letters from a few letters of the alphabet, so c is often repeated,
 * next to itself, or at an end; now and then it occurs only once.
 * Full-size ones have 10^4 letters, and the seed picks where c is: once at
 * the start or once at the end (the largest distances, and the slowest
 * input for searching outwards from every index), only at both ends,
 * everywhere, or scattered, as rarely as one letter in a thousand. */

/* A letter other than c. */
static char other(char c)
{
    int k = (int)rnd(1, 25);

    return (char)('a' + (c - 'a' + k) % 26);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 12) : 10000, used, at, per, i;
    char letters[4], *s = malloc((size_t)n + 1), c;

    if (small) {
        used = (int)rnd(1, 4);
        for (i = 0; i < used; i++)
            letters[i] = (char)('a' + rnd(0, 25));
        c = letters[0];
        for (i = 0; i < n; i++)
            s[i] = letters[rnd(0, used - 1)];
        if (rnd_chance(30)) /* c only once */
            for (i = 0; i < n; i++)
                if (s[i] == c)
                    s[i] = (char)(c == 'z' ? 'a' : c + 1);
        at = (int)rnd(0, n - 1);
        s[at] = c;
    } else {
        c = (char)('a' + rnd(0, 25));
        for (i = 0; i < n; i++)
            s[i] = other(c);
        switch (gen_seed() % 5) {
        case 0: /* once, at the start */
            s[0] = c;
            break;
        case 1: /* once, at the end */
            s[n - 1] = c;
            break;
        case 2: /* only at both ends */
            s[0] = c;
            s[n - 1] = c;
            break;
        case 3: /* everywhere */
            for (i = 0; i < n; i++)
                s[i] = c;
            break;
        default: /* scattered */
            per = (int)rnd(1, 100);
            for (i = 0; i < n; i++)
                if (rnd(1, 1000) <= per)
                    s[i] = c;
            at = (int)rnd(0, n - 1);
            s[at] = c;
            break;
        }
    }
    s[n] = '\0';
    put_string(s);
    put_char(c);
    free(s);
}
