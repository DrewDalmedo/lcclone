/* Small cases are up to 10 letters from a tiny alphabet, so that repeats,
 * lonely letters and -1 all come up often. Full-size cases are 10^5
 * letters, in a shape the seed picks: two halves of one letter each (no
 * lonely letter, and slow for a method that looks for a second copy of
 * each letter from the start); the same with a third letter at the end; 25
 * letters in long blocks and the 26th at the end; random letters from 25
 * and the 26th once, at the start, at the end or anywhere; or random
 * letters from a few to all 26 (almost surely no lonely letter). */
static void letters(int *perm)
{
    int i;

    for (i = 0; i < 26; i++)
        perm[i] = 'a' + i;
    rnd_shuffle(perm, 26);
}

void gen(int small)
{
    static const char *alphabets[] = {
        "ab", "abc", "abcd", "xyz", "az", "abcdefghij"
    };
    int n = small ? (int)rnd(1, 10) : 100000, perm[26], i, k, pos, shape;
    char *s = malloc((size_t)n + 1);
    const char *abc;

    if (small) {
        abc = alphabets[rnd(0, 5)];
        for (i = 0; i < n; i++)
            s[i] = rnd_char(abc);
    } else {
        shape = (int)(gen_seed() % 5);
        letters(perm);
        if (shape <= 1) {
            for (i = 0; i < n; i++)
                s[i] = (char)perm[i < n / 2 ? 0 : 1];
            if (shape == 1)
                s[n - 1] = (char)perm[2];
        } else if (shape == 2) {
            for (i = 0; i < n - 1; i++)
                s[i] = (char)perm[i / 4000];
            s[n - 1] = (char)perm[25];
        } else if (shape == 3) {
            for (i = 0; i < n; i++)
                s[i] = (char)perm[rnd(0, 24)];
            k = (int)rnd(0, 2);
            pos = k == 0 ? 0 : k == 1 ? n - 1 : (int)rnd(0, n - 1);
            s[pos] = (char)perm[25];
        } else {
            k = (int)rnd(2, 26);
            for (i = 0; i < n; i++)
                s[i] = (char)perm[rnd(0, k - 1)];
        }
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
