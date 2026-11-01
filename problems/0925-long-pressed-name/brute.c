/* Slow and obviously right: cut both strings into their runs (maximal
 * blocks of one repeated letter). typed is a stretch of name exactly when
 * the two have the same number of runs, the runs have the same letters in
 * the same order, and no run of typed is shorter than name's. */
static int runs(const char *s, char *letter, int *len)
{
    int n = 0;
    size_t i;

    for (i = 0; s[i] != '\0'; i++) {
        if (i == 0 || s[i] != s[i - 1]) {
            letter[n] = s[i];
            len[n++] = 0;
        }
        len[n - 1]++;
    }
    return n;
}

bool isLongPressedName(char *name, char *typed)
{
    char a[1001], b[1001];
    int la[1001], lb[1001], na, nb, r;

    na = runs(name, a, la);
    nb = runs(typed, b, lb);
    if (na != nb)
        return false;
    for (r = 0; r < na; r++)
        if (a[r] != b[r] || lb[r] < la[r])
            return false;
    return true;
}
