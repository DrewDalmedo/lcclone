/* Reference solution: read s as a ring of n neighbouring pairs, the last
 * one joining s[n - 1] back to s[0], and count the equal pairs, c. Each
 * shift is that ring cut open at one pair: it keeps c - 1 doubles when the
 * cut pair is equal and c when it isn't. So k == c is reached by the n - c
 * cuts at unequal pairs, k == c - 1 by the c cuts at equal pairs, and no
 * other k by any shift. */
int countRotations(char *s, int k)
{
    int n = (int)strlen(s), c = 0, i;

    for (i = 0; i < n; i++)
        if (s[i] == s[(i + 1) % n])
            c++;
    if (k == c)
        return n - c;
    if (k == c - 1)
        return c;
    return 0;
}
