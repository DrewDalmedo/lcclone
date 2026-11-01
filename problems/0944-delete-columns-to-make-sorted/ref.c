/* Reference solution: walk each column from the first string to the last
 * and count it once, as soon as a letter is followed by a smaller one. */
int minDeletionSize(char **strs, int strsSize)
{
    int m = (int)strlen(strs[0]), count = 0, i, j;

    for (j = 0; j < m; j++)
        for (i = 0; i + 1 < strsSize; i++)
            if (strs[i][j] > strs[i + 1][j]) {
                count++;
                break;
            }
    return count;
}
