/* Slow and obviously right: count the zeros and ones of every substring
 * from scratch, and keep the ones with at most k of either. */
int countKConstraintSubstrings(char *s, int k)
{
    int n = (int)strlen(s), start, end, i, zeros, ones, total = 0;

    for (start = 0; start < n; start++)
        for (end = start; end < n; end++) {
            zeros = 0;
            ones = 0;
            for (i = start; i <= end; i++) {
                if (s[i] == '0')
                    zeros++;
                else
                    ones++;
            }
            if (zeros <= k || ones <= k)
                total++;
        }
    return total;
}
