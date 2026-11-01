/* Three strings that start with a shared part. Small cases are up to 5
 * letters of a and b, sharing the first 0 to 3 letters (and often more by
 * chance). Full-size ones are up to 100 letters; the seed picks the shape:
 * 0: three equal strings;
 * 1: a shared part of random length, then random tails, of random lengths;
 * 2: three equal strings except for the first letter of one of them;
 * 3: three strings of 100 letters, sharing only their first letter. */
void gen(int small)
{
    static const char abc[] = "abcdefghijklmnopqrstuvwxyz";
    char base[101], s[3][101];
    int shape = (int)rnd(0, 3), maxlen = small ? 5 : 100, common, len, i, k;
    const char *letters = small ? "ab" : abc;

    for (k = 0; k < maxlen; k++)
        base[k] = rnd_char(letters);
    common = small ? (int)rnd(0, 3) : (int)rnd(1, 100);
    if (!small && shape == 3)
        common = 1;
    len = (int)rnd(1, maxlen);
    for (i = 0; i < 3; i++) {
        if (small || shape == 1)
            len = (int)rnd(1, maxlen);
        else if (shape == 3)
            len = 100;
        for (k = 0; k < len; k++) {
            if (!small && shape != 1 && shape != 3)
                s[i][k] = base[k];
            else if (k < common)
                s[i][k] = base[k];
            else if (!small && shape == 3 && k == 1)
                s[i][k] = abc[i];
            else
                s[i][k] = rnd_char(letters);
        }
        s[i][len] = '\0';
    }
    if (!small && shape == 2) {
        i = (int)rnd(0, 2);
        k = (int)rnd(1, 25);
        s[i][0] = abc[(base[0] - 'a' + k) % 26];
    }
    for (i = 0; i < 3; i++)
        put_string(s[i]);
}
