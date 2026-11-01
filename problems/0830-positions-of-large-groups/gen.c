/* A string of lowercase letters, built as stretches of one letter. Small
 * cases have up to 16 letters, in stretches of 1 to 5 drawn from two or
 * three letters, so neighbouring stretches often share a letter and merge
 * into a longer group, and groups of 2, 3 and 4 come up often. Full-size
 * ones have 1000 letters, in a shape the seed picks: one letter all
 * through (the group [0,999]); groups of exactly 3 in a row, 333 of them
 * (the most there can be); alternating letters (no group at all);
 * stretches of 1 to 6 letters; or stretches of up to 300. */
static void stretches(char *s, int n, int longest, const char *letters)
{
    int i = 0, len, k;
    char ch;

    while (i < n) {
        len = (int)rnd(1, longest);
        ch = rnd_char(letters);
        for (k = 0; k < len && i < n; k++)
            s[i++] = ch;
    }
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 16) : 1000, i;
    char *s = malloc((size_t)n + 1), ch;

    if (small) {
        stretches(s, n, 5, rnd_chance(50) ? "ab" : "xyz");
    } else {
        ch = (char)('a' + rnd(0, 25));
        switch (gen_seed() % 5) {
        case 0: /* one letter */
            for (i = 0; i < n; i++)
                s[i] = ch;
            break;
        case 1: /* groups of 3, taking turns between three letters */
            for (i = 0; i < n; i++)
                s[i] = (char)('a' + (ch - 'a' + i / 3 % 3) % 26);
            break;
        case 2: /* alternating */
            for (i = 0; i < n; i++)
                s[i] = i % 2 ? 'q' : 'p';
            break;
        case 3:
            stretches(s, n, 6, "abcd");
            break;
        default:
            stretches(s, n, 300, "mnopqrstuvwxyz");
            break;
        }
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
