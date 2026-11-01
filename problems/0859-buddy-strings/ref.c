/* Reference solution: the lengths must match. If the words are equal, the
 * swap must trade two equal letters, so some letter must occur twice.
 * Otherwise collect the positions where they differ: there must be exactly
 * two, i and j, with s[i] == goal[j] and s[j] == goal[i]. */
bool buddyStrings(char *s, char *goal)
{
    size_t n = strlen(s), k;
    size_t diff[2];
    int seen[26] = {0}, ndiff = 0;

    if (n != strlen(goal))
        return false;
    for (k = 0; k < n; k++) {
        if (s[k] != goal[k]) {
            if (ndiff == 2)
                return false;
            diff[ndiff++] = k;
        }
    }
    if (ndiff == 0) {
        for (k = 0; k < n; k++) {
            if (seen[s[k] - 'a'])
                return true;
            seen[s[k] - 'a'] = 1;
        }
        return false;
    }
    return ndiff == 2 && s[diff[0]] == goal[diff[1]]
           && s[diff[1]] == goal[diff[0]];
}
