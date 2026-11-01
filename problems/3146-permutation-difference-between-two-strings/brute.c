/* Slow and obviously right: try every pair of places and, whenever the
 * same letter stands at place i of s and place j of t, add |i - j|. */
int findPermutationDifference(char *s, char *t)
{
    int n = (int)strlen(s), i, j, total = 0;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (s[i] == t[j])
                total += i > j ? i - j : j - i;
    return total;
}
