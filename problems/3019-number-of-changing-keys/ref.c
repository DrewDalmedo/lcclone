/* Reference solution: fold every letter to lowercase and count the
 * positions whose folded letter differs from the one just before. */
int countKeyChanges(char *s)
{
    int changes = 0, i;

    for (i = 1; s[i] != '\0'; i++)
        if (tolower((unsigned char)s[i]) != tolower((unsigned char)s[i - 1]))
            changes++;
    return changes;
}
