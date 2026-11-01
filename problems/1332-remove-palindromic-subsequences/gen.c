/* A string of a and b. The seed picks the shape: random letters, a
 * palindrome, a palindrome with one letter changed, a palindrome whose
 * only mismatch is the innermost or the outermost pair, one letter
 * throughout but for a single other letter, or one letter only. Small
 * cases have 1 to 9 letters; full-size ones 999 or 1000, so that a
 * palindrome check has to look at every pair. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 9) : (rnd_chance(50) ? 1000 : 999);
    int shape = (int)rnd(0, 6), i, p;
    char *s = malloc((size_t)n + 1), c, other;

    c = rnd_char("ab");
    other = c == 'a' ? 'b' : 'a';
    for (i = 0; i < n; i++)
        s[i] = c;
    if (shape == 0) {
        for (i = 0; i < n; i++)
            s[i] = rnd_char("ab");
    } else if (shape >= 1 && shape <= 4) {
        for (i = 0; i < (n + 1) / 2; i++) {
            c = rnd_char("ab");
            s[i] = c;
            s[n - 1 - i] = c;
        }
        if (shape == 2) { /* change one letter anywhere */
            p = (int)rnd(0, n - 1);
            s[p] = s[p] == 'a' ? 'b' : 'a';
        } else if (shape == 3 && n >= 2) { /* break the innermost pair */
            p = n / 2 - 1;
            s[p] = s[p] == 'a' ? 'b' : 'a';
        } else if (shape == 4 && n >= 2) { /* break the outermost pair */
            s[0] = s[0] == 'a' ? 'b' : 'a';
        }
    } else if (shape == 5) { /* one other letter, often in the middle */
        p = rnd_chance(50) ? n / 2 : (int)rnd(0, n - 1);
        s[p] = other;
    } /* shape 6 keeps one letter throughout */
    s[n] = '\0';
    put_string(s);
    free(s);
}
