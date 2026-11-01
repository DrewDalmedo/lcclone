/* Reference solution: if s reads the same both ways, one move erases it.
 * Otherwise two moves always do: first every a (a run of equal letters is
 * a palindrome), then every b. One move can't, since it would have to mark
 * the whole of s. So compare s with itself from both ends. */
int removePalindromeSub(char *s)
{
    int i = 0, j = (int)strlen(s) - 1;

    while (i < j) {
        if (s[i] != s[j])
            return 2;
        i++;
        j--;
    }
    return 1;
}
