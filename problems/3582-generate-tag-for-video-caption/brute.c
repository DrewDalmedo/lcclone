/* Slow and obviously right: split a copy of caption into words with
 * strtok, build the whole tag word by word, then cut it to 100
 * characters. */
char *generateTag(char *caption)
{
    size_t n = strlen(caption), len = 1, k;
    char *copy = malloc(n + 1), *tag = malloc(n + 2), *word;
    int first = 1;

    strcpy(copy, caption);
    tag[0] = '#';
    for (word = strtok(copy, " "); word; word = strtok(NULL, " ")) {
        for (k = 0; word[k]; k++) {
            if (k == 0 && !first)
                tag[len++] = (char)toupper((unsigned char)word[k]);
            else
                tag[len++] = (char)tolower((unsigned char)word[k]);
        }
        first = 0;
    }
    tag[len] = '\0';
    if (len > 100)
        tag[100] = '\0';
    free(copy);
    return tag;
}
