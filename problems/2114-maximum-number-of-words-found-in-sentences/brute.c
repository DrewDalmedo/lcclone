/* Slow and obviously right: cut a copy of each sentence into words with
 * strtok, count the pieces, and keep the largest count. */
int mostWordsFound(char **sentences, int sentencesSize)
{
    char copy[101], *tok;
    int best = 0, words, i;

    for (i = 0; i < sentencesSize; i++) {
        strcpy(copy, sentences[i]);
        words = 0;
        for (tok = strtok(copy, " "); tok; tok = strtok(NULL, " "))
            words++;
        if (words > best)
            best = words;
    }
    return best;
}
