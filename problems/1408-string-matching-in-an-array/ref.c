/* Reference solution: join the words with spaces into one text. A word
 * occurs there once as itself, and since no occurrence can cross a space,
 * any second occurrence lies inside another word. So a word hides in
 * another exactly when strstr finds it twice in the text. */
char **stringMatching(char **words, int wordsSize, int *returnSize)
{
    size_t total = 1, len, pos = 0;
    char *text, *first, **out;
    int i;

    for (i = 0; i < wordsSize; i++)
        total += strlen(words[i]) + 1;
    text = malloc(total);
    for (i = 0; i < wordsSize; i++) {
        len = strlen(words[i]);
        memcpy(text + pos, words[i], len);
        pos += len;
        text[pos++] = ' ';
    }
    text[pos] = '\0';

    out = malloc((size_t)wordsSize * sizeof *out);
    *returnSize = 0;
    for (i = 0; i < wordsSize; i++) {
        first = strstr(text, words[i]);
        if (strstr(first + 1, words[i]) != NULL) {
            len = strlen(words[i]);
            out[*returnSize] = malloc(len + 1);
            memcpy(out[*returnSize], words[i], len + 1);
            (*returnSize)++;
        }
    }
    free(text);
    return out;
}
