/* Reference solution: count each letter, then take the largest odd count
 * and the smallest even count among the letters that occur (a count of 0
 * is skipped), and subtract. */
int maxDifference(char *s)
{
    int count[26] = {0};
    int maxOdd = 0, minEven = 101, i;

    for (i = 0; s[i]; i++)
        count[s[i] - 'a']++;
    for (i = 0; i < 26; i++) {
        if (count[i] == 0)
            continue;
        if (count[i] % 2 == 1) {
            if (count[i] > maxOdd)
                maxOdd = count[i];
        } else if (count[i] < minEven) {
            minEven = count[i];
        }
    }
    return maxOdd - minEven;
}
