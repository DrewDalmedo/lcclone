/* Start from a random palindrome, then make each mirror pair disagree, by
 * overwriting its left or its right letter with another one, with a chance
 * the seed picks (0, 40, 70 or 100 in 100), so strings that are already
 * palindromes and strings where every pair disagrees both come up. Small
 * cases have 1 to 8 letters from a few letters the seed picks (near a,
 * near z, both ends, or the middle of the alphabet), so a's, z's and pairs
 * that disagree in either direction are frequent; full-size ones have 999
 * or 1000 letters from the whole alphabet. */
void gen(int small)
{
    static const char *alphabets[] = {
        "ab", "abc", "xyz", "az", "mnop", "abcdefghijklmnopqrstuvwxyz"
    };
    static const int chances[] = {0, 40, 70, 100};
    int n = small ? (int)rnd(1, 8) : (int)rnd(999, 1000);
    int pick = small ? (int)rnd(0, 5) : 5;
    int chance = chances[rnd(0, 3)], at, i;
    const char *letters = alphabets[pick];
    char *s = malloc((size_t)n + 1);
    char c;

    for (i = 0; i <= n - 1 - i; i++) {
        c = rnd_char(letters);
        s[i] = c;
        s[n - 1 - i] = c;
    }
    for (i = 0; i < n - 1 - i; i++)
        if (rnd_chance(chance)) {
            at = rnd_chance(50) ? n - 1 - i : i;
            do
                c = rnd_char(letters);
            while (c == s[at]);
            s[at] = c;
        }
    s[n] = '\0';
    put_string(s);
    free(s);
}
