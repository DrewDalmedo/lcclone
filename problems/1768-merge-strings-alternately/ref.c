/* Reference solution: walk both words with one index, copying a letter
 * from word1 and then one from word2 while each still has letters left,
 * so the leftover of the longer word lands at the end by itself. */
char *mergeAlternately(char *word1, char *word2)
{
    size_t n1 = strlen(word1), n2 = strlen(word2), i, k = 0;
    char *out = malloc(n1 + n2 + 1);

    for (i = 0; i < n1 || i < n2; i++) {
        if (i < n1)
            out[k++] = word1[i];
        if (i < n2)
            out[k++] = word2[i];
    }
    out[k] = '\0';
    return out;
}
