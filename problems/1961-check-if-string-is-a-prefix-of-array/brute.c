/* Slow and obviously right: for each k from 1 to wordsSize, join the
 * first k words together from scratch and compare the text with s. */
bool isPrefixString(char *s, char **words, int wordsSize)
{
    size_t total = 1;
    char *text;
    bool found = false;
    int k, i;

    for (i = 0; i < wordsSize; i++)
        total += strlen(words[i]);
    text = malloc(total);
    for (k = 1; k <= wordsSize; k++) {
        text[0] = '\0';
        for (i = 0; i < k; i++)
            strcat(text, words[i]);
        if (strcmp(text, s) == 0)
            found = true;
    }
    free(text);
    return found;
}
