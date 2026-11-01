/* Slow and obviously right: for each letter of the alphabet, find its
 * first and last copy in s, if it has any, and count the characters
 * between them one by one. */
bool checkDistances(char *s, int *distance, int distanceSize)
{
    int n = (int)strlen(s), c, i, j, k, between;

    (void)distanceSize;
    for (c = 0; c < 26; c++) {
        for (i = 0; i < n && s[i] != 'a' + c; i++)
            ;
        if (i == n)
            continue;
        for (j = n - 1; s[j] != 'a' + c; j--)
            ;
        between = 0;
        for (k = i + 1; k < j; k++)
            between++;
        if (between != distance[c])
            return false;
    }
    return true;
}
