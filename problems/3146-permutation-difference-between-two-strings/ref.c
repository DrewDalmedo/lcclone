/* Reference solution: record where each letter stands in t, then add up
 * the distance from its place in s, with signed ints so that letters
 * moving left and right are both counted correctly. */
int findPermutationDifference(char *s, char *t)
{
    int where[26], i, d, total = 0;

    for (i = 0; t[i] != '\0'; i++)
        where[t[i] - 'a'] = i;
    for (i = 0; s[i] != '\0'; i++) {
        d = i - where[s[i] - 'a'];
        total += d < 0 ? -d : d;
    }
    return total;
}
