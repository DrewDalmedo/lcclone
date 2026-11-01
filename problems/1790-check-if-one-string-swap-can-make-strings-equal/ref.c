/* Reference solution: only the positions where the strings differ matter.
 * None means they are equal already; exactly two, i and j, can be fixed
 * when the letters cross (s1[i] == s2[j] and s1[j] == s2[i]); any other
 * count cannot. Stop at the third difference. */
bool areAlmostEqual(char *s1, char *s2)
{
    int diff[2], count = 0, k;

    for (k = 0; s1[k] != '\0'; k++) {
        if (s1[k] == s2[k])
            continue;
        if (count == 2)
            return false;
        diff[count++] = k;
    }
    if (count == 0)
        return true;
    if (count == 1)
        return false;
    return s1[diff[0]] == s2[diff[1]] && s1[diff[1]] == s2[diff[0]];
}
