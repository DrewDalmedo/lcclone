/* Slow and obviously right: glue each array into a string of its own and
 * compare the two with strcmp. */
static char *glue(char **words, int count)
{
    size_t len = 0;
    char *s;
    int i;

    for (i = 0; i < count; i++)
        len += strlen(words[i]);
    s = malloc(len + 1);
    s[0] = '\0';
    for (i = 0; i < count; i++)
        strcat(s, words[i]);
    return s;
}

bool arrayStringsAreEqual(char **word1, int word1Size, char **word2,
                          int word2Size)
{
    char *a = glue(word1, word1Size), *b = glue(word2, word2Size);
    bool same = strcmp(a, b) == 0;

    free(a);
    free(b);
    return same;
}
