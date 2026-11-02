/* Slow and obviously right: for every pair of neighbours, try each of the
 * five values the second digit may take (the first digit -2, -1, 0, +1 or
 * +2) and fail when none of them is the second digit. */
bool isAdjacentDiffAtMostTwo(char *s)
{
    int n = (int)strlen(s), i, k, found;

    for (i = 0; i + 1 < n; i++) {
        found = 0;
        for (k = -2; k <= 2; k++)
            if ((s[i] - '0') + k == s[i + 1] - '0')
                found = 1;
        if (!found)
            return false;
    }
    return true;
}
