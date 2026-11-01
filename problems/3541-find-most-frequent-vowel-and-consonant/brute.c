/* Slow and obviously right: for each position, count how many positions
 * hold the same letter, and keep the largest such count seen for a vowel
 * and for a consonant. */
int maxFreqSum(char *s)
{
    int n = (int)strlen(s), vmax = 0, cmax = 0, i, j, cnt;

    for (i = 0; i < n; i++) {
        cnt = 0;
        for (j = 0; j < n; j++)
            if (s[j] == s[i])
                cnt++;
        if (strchr("aeiou", s[i]) != NULL) {
            if (cnt > vmax)
                vmax = cnt;
        } else if (cnt > cmax) {
            cmax = cnt;
        }
    }
    return vmax + cmax;
}
