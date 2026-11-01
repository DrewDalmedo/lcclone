/* Reference solution: equal strings have the same subsequences, so then
 * there is none that is uncommon. Otherwise the longer string (either one
 * when the lengths are equal) is a subsequence of itself but not of the
 * other: a subsequence of the other is no longer than it, and only as long
 * when it is the other string itself. Nothing longer than both can be a
 * subsequence of either, so that length is the answer. */
int findLUSlength(char *a, char *b)
{
    int la = (int)strlen(a), lb = (int)strlen(b);

    if (strcmp(a, b) == 0)
        return -1;
    return la > lb ? la : lb;
}
