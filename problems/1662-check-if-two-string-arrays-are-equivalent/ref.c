/* Reference solution: walk both arrays at once, keeping on each side the
 * string being read and the position in it, and compare one character at
 * a time without building the glued text. The answer is true only if both
 * sides run out together. */
bool arrayStringsAreEqual(char **word1, int word1Size, char **word2,
                          int word2Size)
{
    int i = 0, j = 0, p = 0, q = 0;

    while (i < word1Size && j < word2Size) {
        if (word1[i][p] != word2[j][q])
            return false;
        if (word1[i][++p] == '\0') {
            i++;
            p = 0;
        }
        if (word2[j][++q] == '\0') {
            j++;
            q = 0;
        }
    }
    return i == word1Size && j == word2Size;
}
