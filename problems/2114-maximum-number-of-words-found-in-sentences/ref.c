/* Reference solution: with single spaces and none at the ends, a sentence
 * has one word more than it has spaces; count the spaces of each sentence
 * and keep the largest count plus one. */
int mostWordsFound(char **sentences, int sentencesSize)
{
    int best = 0, words, i;
    const char *p;

    for (i = 0; i < sentencesSize; i++) {
        words = 1;
        for (p = sentences[i]; *p; p++)
            if (*p == ' ')
                words++;
        if (words > best)
            best = words;
    }
    return best;
}
