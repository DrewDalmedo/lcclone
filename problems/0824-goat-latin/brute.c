/* Slow and obviously right: split a copy of the sentence at its spaces,
 * build each new word in a buffer of its own (the rest of the word, then
 * its first letter, for a consonant), and strcat the pieces together. */
char *toGoatLatin(char *sentence)
{
    char *copy = malloc(strlen(sentence) + 1), *word, piece[200];
    char *out = malloc(4000);
    int i = 0, k;

    strcpy(copy, sentence);
    out[0] = '\0';
    for (word = strtok(copy, " "); word; word = strtok(NULL, " ")) {
        i++;
        if (strchr("aeiouAEIOU", word[0])) {
            strcpy(piece, word);
        } else {
            strcpy(piece, word + 1);
            strncat(piece, word, 1);
        }
        strcat(piece, "m");
        for (k = 0; k <= i; k++)
            strcat(piece, "a");
        if (i > 1)
            strcat(out, " ");
        strcat(out, piece);
    }
    free(copy);
    return out;
}
