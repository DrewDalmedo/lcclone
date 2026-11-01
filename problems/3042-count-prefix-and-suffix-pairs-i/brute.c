/* Slow and obviously right, by another route: for each later word b, copy
 * out every way of keeping its first m letters and every way of keeping
 * its last m letters, keep the strings that are on both lists, and count
 * the earlier words equal to one of them. */
int countPrefixSuffixPairs(char **words, int wordsSize)
{
    char both[10][11], head[11], tail[11];
    int count = 0, nboth, i, j, m, k, lb;

    for (j = 1; j < wordsSize; j++) {
        lb = (int)strlen(words[j]);
        nboth = 0;
        for (m = 1; m <= lb; m++) {
            for (k = 0; k < m; k++) {
                head[k] = words[j][k];
                tail[k] = words[j][lb - m + k];
            }
            head[m] = '\0';
            tail[m] = '\0';
            if (strcmp(head, tail) == 0)
                strcpy(both[nboth++], head);
        }
        for (i = 0; i < j; i++)
            for (k = 0; k < nboth; k++)
                if (strcmp(words[i], both[k]) == 0)
                    count++;
    }
    return count;
}
