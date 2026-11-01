/* Reference solution: one pass over s, remembering where each letter was
 * first seen (-1 for not yet); at a letter's second copy, compare the
 * characters in between with its entry of distance. */
bool checkDistances(char *s, int *distance, int distanceSize)
{
    int first[26], i, c;

    (void)distanceSize;
    for (c = 0; c < 26; c++)
        first[c] = -1;
    for (i = 0; s[i] != '\0'; i++) {
        c = s[i] - 'a';
        if (first[c] < 0)
            first[c] = i;
        else if (i - first[c] - 1 != distance[c])
            return false;
    }
    return true;
}
