/* Slow and obviously right: cut a copy of text into words, and for each
 * word compare every one of its letters with every broken letter. */
int canBeTypedWords(char *text, char *brokenLetters)
{
    size_t n = strlen(text), i, j;
    char *copy = malloc(n + 1), *word;
    int whole = 0, ok;

    memcpy(copy, text, n + 1);
    for (word = strtok(copy, " "); word != NULL; word = strtok(NULL, " ")) {
        ok = 1;
        for (i = 0; word[i] != '\0'; i++)
            for (j = 0; brokenLetters[j] != '\0'; j++)
                if (word[i] == brokenLetters[j])
                    ok = 0;
        whole += ok;
    }
    free(copy);
    return whole;
}
