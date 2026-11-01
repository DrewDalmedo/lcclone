/* Random letters and a random group length k. Small cases have 1 to 10
 * letters from a few; k is usually at most half the length (rounded up),
 * so that there are several groups, and otherwise from the length to 12.
 * Full-size ones have 10^4 letters, and the seed picks k: 1, 10^4 (one
 * group), 2500 (four pairs of groups, nothing left over), n / 2 + 1 (a
 * reversed group and a shorter one that stays), a few letters, or any
 * length. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 10) : 10000, k, i;
    char *s = malloc((size_t)n + 1);

    for (i = 0; i < n; i++)
        s[i] = small ? rnd_char("hijkl") : (char)rnd('a', 'z');
    s[n] = '\0';
    if (small) {
        k = rnd_chance(25) ? (int)rnd(n, 12) : (int)rnd(1, (n + 1) / 2);
    } else {
        switch (gen_seed() % 6) {
        case 0: k = 1; break;
        case 1: k = 10000; break;
        case 2: k = 2500; break;
        case 3: k = n / 2 + 1; break;
        case 4: k = (int)rnd(2, 40); break;
        default: k = (int)rnd(1, 10000); break;
        }
    }
    put_string(s);
    put_int(k);
    free(s);
}
