/* Slow and obviously right: compare every pair of positions, which must be
 * equal in s exactly when they are equal in t. */
bool isIsomorphic(char *s, char *t)
{
    size_t n = strlen(s), i, j;

    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if ((s[i] == s[j]) != (t[i] == t[j]))
                return false;
    return true;
}
