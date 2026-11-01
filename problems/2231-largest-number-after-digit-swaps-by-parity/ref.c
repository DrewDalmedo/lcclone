/* Reference solution: count how often each digit occurs. Then walk num's
 * places from the most significant one, and give each place the largest
 * digit still unused from the family (odd or even) of the digit that was
 * there. */
int largestInteger(int num)
{
    char d[16];
    int count[10] = {0}, n, i, v, ans = 0;

    n = snprintf(d, sizeof d, "%d", num);
    for (i = 0; i < n; i++)
        count[d[i] - '0']++;
    for (i = 0; i < n; i++) {
        v = (d[i] - '0') % 2 ? 9 : 8;
        while (count[v] == 0)
            v -= 2;
        count[v]--;
        ans = ans * 10 + v;
    }
    return ans;
}
