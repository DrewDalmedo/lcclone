/* Slow and obviously right: for every letter that occurs in either word,
 * count it in both words by looking at each of their letters, and compare
 * the two counts. A letter in neither word has counts 0 and 0. */
static int occurrences(const char *word, char c)
{
    int count = 0, i;

    for (i = 0; word[i] != '\0'; i++)
        if (word[i] == c)
            count++;
    return count;
}

bool checkAlmostEquivalent(char *word1, char *word2)
{
    const char *words[2] = {word1, word2};
    int w, i, gap;

    for (w = 0; w < 2; w++)
        for (i = 0; words[w][i] != '\0'; i++) {
            gap = occurrences(word1, words[w][i]) -
                  occurrences(word2, words[w][i]);
            if (gap > 3 || gap < -3)
                return false;
        }
    return true;
}
