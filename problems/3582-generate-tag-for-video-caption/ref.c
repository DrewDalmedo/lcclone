/* Reference solution: one pass over caption. A letter that follows a
 * non-letter (or the start) begins a word: it is uppercased unless it is
 * the first word's, and every other letter is lowercased. Letters stop
 * being added once the tag holds 100 characters. */
char *generateTag(char *caption)
{
    char *tag = malloc(101);
    int len = 1, words = 0, prevLetter = 0, i;

    tag[0] = '#';
    for (i = 0; caption[i] && len < 100; i++) {
        unsigned char c = (unsigned char)caption[i];

        if (!isalpha(c)) {
            prevLetter = 0;
            continue;
        }
        if (!prevLetter && words++ > 0)
            tag[len++] = (char)toupper(c);
        else
            tag[len++] = (char)tolower(c);
        prevLetter = 1;
    }
    tag[len] = '\0';
    return tag;
}
