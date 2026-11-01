/* Slow and obviously right: try every pair i < j, and check each letter
 * from a to z: it must be in both words or in neither. */
int similarPairs(char **words, int wordsSize)
{
    int pairs = 0, i, j, same;
    char c;

    for (i = 0; i < wordsSize; i++)
        for (j = i + 1; j < wordsSize; j++) {
            same = 1;
            for (c = 'a'; c <= 'z'; c++)
                if ((strchr(words[i], c) != NULL)
                    != (strchr(words[j], c) != NULL))
                    same = 0;
            pairs += same;
        }
    return pairs;
}
