/* A string and a group size. Small cases use few letters (so fill often
 * matches letters of s) and k up to 10, sometimes past the length; full
 * size ones pick the shape from the seed: s of 100 letters, k a divisor
 * of the length, k past the length, or both at random. */
void gen(int small)
{
    char s[101];
    int shape = (int)rnd(0, 3), n, k, i;
    const char *letters = small ? "abc" : "abcdefghijklmnopqrstuvwxyz";
    char fill;

    if (small) {
        n = (int)rnd(1, 8);
        k = (int)rnd(1, 10);
    } else if (shape == 0) {
        n = 100;
        k = (int)rnd(1, 100);
    } else if (shape == 1) {
        n = (int)rnd(1, 100);
        do
            k = (int)rnd(1, n);
        while (n % k != 0);
    } else if (shape == 2) {
        n = (int)rnd(1, 99);
        k = (int)rnd(n + 1, 100);
    } else {
        n = (int)rnd(1, 100);
        k = (int)rnd(1, 100);
    }
    for (i = 0; i < n; i++)
        s[i] = rnd_char(letters);
    s[n] = '\0';
    fill = rnd_char(letters);
    put_string(s);
    put_int(k);
    put_char(fill);
}
