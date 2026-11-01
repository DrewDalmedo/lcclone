/* A string with each letter of a chosen set at least once, the rest of it
 * drawn from that set, in random order. The set is the whole alphabet (a
 * pangram) or the alphabet less one letter (a, z or one in between).
 * Small cases are 25 to 34 letters long, or a few letters of a, b and z;
 * full-size ones are 1000 letters, and the seed picks a pangram, one
 * whose only z is its last letter (the slowest case for a search that
 * looks for a, b, ..., z in turn), or one letter missing. */
static void fill(char *s, int n, const char *set)
{
    int a[1000], m = (int)strlen(set), i, k;

    for (i = 0; i < n; i++) {
        k = i < m ? i : (int)rnd(0, m - 1);
        a[i] = set[k];
    }
    rnd_shuffle(a, n);
    for (i = 0; i < n; i++)
        s[i] = (char)a[i];
    s[n] = '\0';
}

/* The alphabet without one letter: a, z or one of b to y. */
static void all_but_one(char *set)
{
    int which = (int)rnd(0, 2), skip, i, m = 0;

    if (which == 0)
        skip = 'a';
    else if (which == 1)
        skip = 'z';
    else
        skip = (int)rnd('b', 'y');
    for (i = 'a'; i <= 'z'; i++)
        if (i != skip)
            set[m++] = (char)i;
    set[m] = '\0';
}

void gen(int small)
{
    const char *alphabet = "abcdefghijklmnopqrstuvwxyz";
    char s[1001], set[27];
    int shape = (int)rnd(0, 4), n, i;

    if (small) {
        if (shape == 0) {
            n = (int)rnd(1, 8);
            for (i = 0; i < n; i++)
                s[i] = rnd_char("abz");
            s[n] = '\0';
        } else if (shape <= 2) {
            n = (int)rnd(26, 34);
            fill(s, n, alphabet);
        } else {
            all_but_one(set);
            n = (int)rnd(25, 34);
            fill(s, n, set);
        }
    } else {
        if (shape <= 1) {
            fill(s, 1000, alphabet);
        } else if (shape == 2) {
            fill(s, 999, "abcdefghijklmnopqrstuvwxy");
            s[999] = 'z';
            s[1000] = '\0';
        } else {
            all_but_one(set);
            fill(s, 1000, set);
        }
    }
    put_string(s);
}
