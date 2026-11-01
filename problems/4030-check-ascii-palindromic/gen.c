/* A string of lowercase letters. Few strings of random letters pass, so
 * the seed picks a shape: the mirror pattern of letters whose bytes pass
 * (a byte matches its partner's reversed: 'f' with 'f', 'n' with 'v');
 * that pattern with one letter changed; random letters from a few that
 * pass and a few that nearly do; a palindrome of letters; or any letters.
 * Small cases have up to 8 letters, full-size ones 99 or 100. */
static char partner(char c)
{
    return c == 'n' ? 'v' : c == 'v' ? 'n' : c;
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : (int)rnd(99, 100);
    int pick = (int)rnd(0, 99), shape, i, pos;
    char *s = malloc((size_t)n + 1);
    char c;

    if (pick < 35)
        shape = 0;
    else if (pick < 65)
        shape = 1;
    else if (pick < 80)
        shape = 2;
    else if (pick < 92)
        shape = 3;
    else
        shape = 4;
    if (shape <= 1) {
        for (i = 0; i < n / 2; i++) {
            c = rnd_char("fnv");
            s[i] = c;
            s[n - 1 - i] = partner(c);
        }
        if (n % 2 == 1)
            s[n / 2] = 'f';
        if (shape == 1) {
            pos = (int)rnd(0, n - 1);
            do {
                c = rnd_char("fnvfnvcgkwab");
            } while (c == s[pos]);
            s[pos] = c;
        }
    } else if (shape == 2) {
        for (i = 0; i < n; i++)
            s[i] = rnd_char("fnvcgkw");
    } else if (shape == 3) {
        for (i = 0; i < (n + 1) / 2; i++) {
            c = rnd_char("abfnvwz");
            s[i] = c;
            s[n - 1 - i] = c;
        }
    } else {
        for (i = 0; i < n; i++)
            s[i] = (char)rnd('a', 'z');
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
