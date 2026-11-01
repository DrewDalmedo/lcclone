/* Slow and obviously right: for every letter of every word, look through
 * allowed letter by letter; a word counts when every search succeeds. */
int countConsistentStrings(char *allowed, char **words, int wordsSize)
{
    int count = 0, ok, found, i, j, k;

    for (i = 0; i < wordsSize; i++) {
        ok = 1;
        for (j = 0; words[i][j] != '\0'; j++) {
            found = 0;
            for (k = 0; allowed[k] != '\0'; k++)
                if (allowed[k] == words[i][j])
                    found = 1;
            if (!found)
                ok = 0;
        }
        if (ok)
            count++;
    }
    return count;
}
