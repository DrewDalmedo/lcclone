/* Slow and obviously right: a row of 0s and 1s whose neighbours always
 * differ is 0101... or 1010.... So for each prefix, count its 0s anew,
 * build both rows of its length, and see whether either has that many 0s
 * (and so the same characters as the prefix). */
int countValidPrefixes(char *s)
{
    int n = (int)strlen(s), k, i, first, zeros, rowZeros, count = 0;
    char row[101];

    for (k = 1; k <= n; k++) {
        zeros = 0;
        for (i = 0; i < k; i++)
            if (s[i] == '0')
                zeros++;
        for (first = 0; first < 2; first++) {
            rowZeros = 0;
            for (i = 0; i < k; i++) {
                row[i] = (char)('0' + (first + i) % 2);
                if (row[i] == '0')
                    rowZeros++;
            }
            if (rowZeros == zeros) {
                count++;
                break;
            }
        }
    }
    return count;
}
