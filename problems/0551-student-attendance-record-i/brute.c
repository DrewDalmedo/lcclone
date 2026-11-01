/* Slow and obviously right, straight from the rules: compare every pair of
 * days for two absences, and look at every three neighbouring days for
 * three late ones. */
bool checkRecord(char *s)
{
    int n = (int)strlen(s), i, j;

    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (s[i] == 'A' && s[j] == 'A')
                return false;
    for (i = 0; i + 2 < n; i++)
        if (s[i] == 'L' && s[i + 1] == 'L' && s[i + 2] == 'L')
            return false;
    return true;
}
