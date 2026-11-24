/* Right, but too slow at full size: find the first three letters in a row,
 * strike one, and start looking again from the front. On the full-size case
 * of half the length with no three in a row, then one long run, each of the
 * 5 * 10^4 strikes rescans the clean half first: about 2.5 * 10^9 steps. */
char *makeFancyString(char *s)
{
    size_t n = strlen(s), i;
    int struck = 1;

    while (struck) {
        struck = 0;
        for (i = 2; i < n; i++)
            if (s[i] == s[i - 1] && s[i] == s[i - 2]) {
                memmove(s + i, s + i + 1, n - i);
                n--;
                struck = 1;
                break;
            }
    }
    return s;
}
