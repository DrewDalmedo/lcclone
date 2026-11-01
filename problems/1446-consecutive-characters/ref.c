/* Reference solution: one pass, counting how long the current run of equal
 * letters is and keeping the longest count seen. */
int maxPower(char *s)
{
    int best = 1, run = 1, i;

    for (i = 1; s[i]; i++) {
        run = s[i] == s[i - 1] ? run + 1 : 1;
        if (run > best)
            best = run;
    }
    return best;
}
