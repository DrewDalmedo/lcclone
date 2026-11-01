/* A string and a permutation of its positions. Small cases have up to 8
 * letters, often from a two- or three-letter alphabet so that repeats come
 * up; full-size ones have 100 letters. The seed picks the permutation: a
 * random one, a rotation, or a single cycle through every position
 * (Sattolo's shuffle), where reading indices the wrong way round fails. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, i, j, t;
    int shape = (int)rnd(0, 2), shift;
    const char *letters;
    char s[101];
    int idx[100];

    if (small) {
        t = (int)rnd(0, 2);
        letters = t == 0 ? "ab" : t == 1 ? "xyz" : "abcdefghijklmnopqrstuvwxyz";
    } else {
        letters = "abcdefghijklmnopqrstuvwxyz";
    }
    for (i = 0; i < n; i++)
        s[i] = rnd_char(letters);
    s[n] = '\0';
    for (i = 0; i < n; i++)
        idx[i] = i;
    if (shape == 0) {
        rnd_shuffle(idx, n);
    } else if (shape == 1) {
        shift = (int)rnd(0, n - 1);
        for (i = 0; i < n; i++)
            idx[i] = (i + shift) % n;
    } else {
        for (i = n - 1; i > 0; i--) {
            j = (int)rnd(0, i - 1);
            t = idx[i];
            idx[i] = idx[j];
            idx[j] = t;
        }
    }
    put_string(s);
    put_ints(idx, n);
}
