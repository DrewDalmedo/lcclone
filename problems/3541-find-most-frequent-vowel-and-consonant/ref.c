/* Reference solution: count every letter in one pass, then take the
 * largest count among the five vowels and the largest among the other
 * 21 letters, and add the two. */
int maxFreqSum(char *s)
{
    static const char is_vowel[26] = {
        ['a' - 'a'] = 1, ['e' - 'a'] = 1, ['i' - 'a'] = 1,
        ['o' - 'a'] = 1, ['u' - 'a'] = 1,
    };
    int cnt[26] = {0};
    int vmax = 0, cmax = 0, c;
    const char *p;

    for (p = s; *p; p++)
        cnt[*p - 'a']++;
    for (c = 0; c < 26; c++) {
        if (is_vowel[c]) {
            if (cnt[c] > vmax)
                vmax = cnt[c];
        } else if (cnt[c] > cmax) {
            cmax = cnt[c];
        }
    }
    return vmax + cmax;
}
