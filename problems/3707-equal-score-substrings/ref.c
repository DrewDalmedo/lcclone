/* Reference solution: add up the score of the whole string, then grow the
 * left substring one letter at a time and stop when its score is exactly
 * half the total (twice the left score equals the total, which also rules
 * out an odd total). The right substring must keep at least one letter. */
bool scoreBalance(char *s)
{
    int n = (int)strlen(s), total = 0, left = 0, i;

    for (i = 0; i < n; i++)
        total += s[i] - 'a' + 1;
    for (i = 0; i < n - 1; i++) {
        left += s[i] - 'a' + 1;
        if (2 * left == total)
            return true;
    }
    return false;
}
