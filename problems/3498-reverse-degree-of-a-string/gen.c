/* Random lowercase strings. Small ones often use only a few letters, so
 * repeats and the extreme weights come up; full-size ones have 1000
 * characters, sometimes all 'a' (the largest total) or all 'z'. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000, i;
    int shape = (int)rnd(0, 5);
    const char *letters = "abcdefghijklmnopqrstuvwxyz";
    char *s = malloc((size_t)n + 1);

    if (shape == 0)
        letters = small ? "az" : "a";
    else if (shape == 1)
        letters = small ? "xyz" : "z";
    else if (shape == 2)
        letters = small ? "abc" : "az";
    for (i = 0; i < n; i++)
        s[i] = rnd_char(letters);
    s[n] = '\0';
    put_string(s);
    free(s);
}
