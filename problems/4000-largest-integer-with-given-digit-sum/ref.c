/* Reference solution: n digits sum to at most 9 * n, so a larger s has no
 * answer. Otherwise use all n digits (more digits make a larger number) and
 * fill them from the left, each with as much of s as it can hold: 9, or
 * what is left of s. */
int largestInteger(int n, int s)
{
    int ans = 0, i, d;

    if (s > 9 * n)
        return -1;
    for (i = 0; i < n; i++) {
        d = s < 9 ? s : 9;
        ans = ans * 10 + d;
        s -= d;
    }
    return ans;
}
